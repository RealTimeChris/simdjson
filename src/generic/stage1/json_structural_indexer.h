#ifndef SIMDJSON2_SRC_GENERIC_STAGE1_JSON_STRUCTURAL_INDEXER_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_SRC_GENERIC_STAGE1_JSON_STRUCTURAL_INDEXER_H
#include <generic/stage1/base.h>
#include <generic/stage1/buf_block_reader.h>
#include <generic/stage1/find_next_document_index.h>
#include <generic/stage1/json_minifier.h>
#include <generic/stage1/json_scanner.h>
#include <generic/stage1/json_string_scanner.h>
#include <generic/stage1/utf8_lookup4_algorithm.h>
#endif // SIMDJSON2_CONDITIONAL_INCLUDE
#include <algorithm>

// This file contains the common code every implementation uses in stage1
// It is intended to be included multiple times and compiled multiple times
// We assume the file in which it is included already includes
// "simdjson2/stage1.h" (this simplifies amalgation)

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace {
namespace stage1 {

class bit_indexer {
public:
  uint32_t *tail;

  simdjson2_really_inline bit_indexer(uint32_t *index_buf) : tail(index_buf) {}

#if SIMDJSON2_PREFER_REVERSE_BITS
  /**
   * ARM lacks a fast trailing zero instruction, but it has a fast
   * bit reversal instruction and a fast leading zero instruction.
   * Thus it may be profitable to reverse the bits (once) and then
   * to rely on a sequence of instructions that call the leading
   * zero instruction.
   *
   * Performance notes:
   * The chosen routine is not optimal in terms of data dependency
   * since zero_leading_bit might require two instructions. However,
   * it tends to minimize the total number of instructions which is
   * beneficial.
   */
  simdjson2_really_inline void write_index(uint32_t idx, uint64_t &rev_bits,
                                           int i) {
    int lz = leading_zeroes(rev_bits);
    this->tail[i] = static_cast<uint32_t>(idx) + lz;
    rev_bits = zero_leading_bit(rev_bits, lz);
  }
#else
  /**
   * Under recent x64 systems, we often have both a fast trailing zero
   * instruction and a fast 'clear-lower-bit' instruction so the following
   * algorithm can be competitive.
   */

  simdjson2_really_inline void write_index(uint32_t idx, uint64_t &bits,
                                           int i) {
    this->tail[i] = idx + trailing_zeroes(bits);
    bits = clear_lowest_bit(bits);
  }
#endif // SIMDJSON2_PREFER_REVERSE_BITS

  template <int START, int N>
  simdjson2_really_inline int write_indexes(uint32_t idx, uint64_t &bits) {
    write_index(idx, bits, START);
    SIMDJSON2_IF_CONSTEXPR(N > 1) {
      write_indexes<(N - 1 > 0 ? START + 1 : START), (N - 1 >= 0 ? N - 1 : 1)>(
          idx, bits);
    }
    return START + N;
  }

  template <int START, int END, int STEP>
  simdjson2_really_inline int write_indexes_stepped(uint32_t idx,
                                                    uint64_t &bits, int cnt) {
    write_indexes<START, STEP>(idx, bits);
    SIMDJSON2_IF_CONSTEXPR((START + STEP) < END) {
      if (simdjson2_unlikely((START + STEP) < cnt)) {
        write_indexes_stepped<(START + STEP < END ? START + STEP : END), END,
                              STEP>(idx, bits, cnt);
      }
    }
    return ((END - START) % STEP) == 0
               ? END
               : (END - START) - ((END - START) % STEP) + STEP;
  }

  // flatten out values in 'bits' assuming that they are are to have values of
  // idx plus their position in the bitvector, and store these indexes at
  // base_ptr[base] incrementing base as we go
  // will potentially store extra values beyond end of valid bits, so base_ptr
  // needs to be large enough to handle this
  //
  // If the kernel sets
  // SIMDJSON2_GENERIC_JSON_STRUCTURAL_INDEXER_CUSTOM_BIT_INDEXER, then it will
  // provide its own version of the code.
#ifdef SIMDJSON2_GENERIC_JSON_STRUCTURAL_INDEXER_CUSTOM_BIT_INDEXER
  simdjson2_really_inline void write(uint32_t idx, uint64_t bits);
#else
  simdjson2_really_inline void write(uint32_t idx, uint64_t bits) {
    // In some instances, the next branch is expensive because it is
    // mispredicted. Unfortunately, in other cases, it helps tremendously.
    if (bits == 0)
      return;

    int cnt = static_cast<int>(count_ones(bits));

#if SIMDJSON2_PREFER_REVERSE_BITS
    bits = reverse_bits(bits);
#endif
#ifdef SIMDJSON2_STRUCTURAL_INDEXER_STEP
    static constexpr const int STEP = SIMDJSON2_STRUCTURAL_INDEXER_STEP;
#else
    static constexpr const int STEP = 4;
#endif
    static constexpr const int STEP_UNTIL = 24;

    write_indexes_stepped<0, STEP_UNTIL, STEP>(idx, bits, cnt);
    SIMDJSON2_IF_CONSTEXPR(STEP_UNTIL < 64) {
      if (simdjson2_unlikely(STEP_UNTIL < cnt)) {
        for (int i = STEP_UNTIL; i < cnt; i++) {
          write_index(idx, bits, i);
        }
      }
    }

    this->tail += cnt;
  }
#endif // SIMDJSON2_GENERIC_JSON_STRUCTURAL_INDEXER_CUSTOM_BIT_INDEXER
};

class string_block_reader {
public:
  simdjson2_really_inline void reset(const uint8_t *stringViewNew,
                                     uint64_t lengthNew) noexcept {
    lengthMinusStep = lengthNew < 256 ? 0 : lengthNew - 256;
    inString = stringViewNew;
    length = lengthNew;
    index = 0;
  }

  simdjson2_really_inline uint64_t getRemainder(uint8_t *dest) const noexcept {
    if ((length == index)) {
      return 0;
    }
    std::fill_n(dest, 256, static_cast<char>(0x20));
    std::copy_n(inString + index, length - index, dest);
    return length - index;
  }

  simdjson2_really_inline const uint8_t *fullBlock() noexcept {
    const uint8_t *newPtr = inString + index;
    index += 256;
    return newPtr;
  }

  simdjson2_really_inline bool hasFullBlock() const noexcept {
    return index < lengthMinusStep;
  }

protected:
  const uint8_t *inString{};
  uint64_t lengthMinusStep{};
  uint64_t length{};
  uint64_t index{};
};

template <auto multiple, typename value_type = decltype(multiple)>
simdjson2_really_inline constexpr value_type
roundUpToMultiple(value_type value) noexcept {
  if constexpr ((multiple & (multiple - 1)) == 0) {
    return (value + (multiple - 1)) & ~(multiple - 1);
  } else {
    auto remainder = value % multiple;
    return remainder == 0 ? value : value + (multiple - remainder);
  }
}

template <auto multiple, typename value_type = decltype(multiple)>
simdjson2_really_inline constexpr value_type
roundDownToMultiple(value_type value) noexcept {
  if constexpr ((multiple & (multiple - 1)) == 0) {
    return value & ~(multiple - 1);
  } else {
    return static_cast<int64_t>(value) >= 0
               ? (value / multiple) * multiple
               : ((value - multiple + 1) / multiple) * multiple;
  }
}

class json_structural_indexer : public std::allocator<uint32_t *> {
public:
  using size_type = uint64_t;
  using allocator = std::allocator<uint32_t *>;
  static constexpr double multiplier{4.5f / 5.0f};

  simdjson2_really_inline json_structural_indexer() noexcept {};

  template <size_t size>
  simdjson2_really_inline error_code index(const uint8_t *buf, size_t len,
                                           dom_parser_implementation &parserNew,
                                           stage1_mode partial) noexcept {
    parser = &parserNew;
    currentParseBuffer =
        std::basic_string_view{static_cast<const uint8_t *>(buf), len};
    resetImpl();
    return error_code::SUCCESS;
  }

  simdjson2_really_inline ~json_structural_indexer() noexcept {}

protected:
  dom_parser_implementation *parser{};
  alignas(32) size_type newBits[4]{};
  std::basic_string_view<uint8_t> currentParseBuffer{};
  alignas(32) uint8_t block[256]{};
  string_block_reader stringBlockReader{};
  size_type stringIndex{};
  int64_t prevInString{};
  bool overflow{};

  template <bool minified = false>
  simdjson2_really_inline void resetImpl() noexcept {
    stringBlockReader.reset(currentParseBuffer.data(),
                            currentParseBuffer.size());
    overflow = false;
    prevInString = 0;
    stringIndex = 0;
    parser->n_structural_indexes = 0;
    generateJsonIndices<minified>();
  }

  template <bool minified>
  simdjson2_really_inline void generateJsonIndices() noexcept {
    simd_int_t_holder rawStructurals{};
    simd8<uint8_t> nextIsEscaped{};
    simd8<uint8_t> escaped{};
    while (stringBlockReader.hasFullBlock()) {
      generateStructurals<false, minified>(stringBlockReader.fullBlock(),
                                           escaped, nextIsEscaped,
                                           rawStructurals);
    }
    if ((stringBlockReader.getRemainder(block) > 0)) {
      generateStructurals<true, minified>(block, escaped, nextIsEscaped,
                                          rawStructurals);
    }
  }

  template <size_type index>
  simdjson2_really_inline size_type
  rollValuesIntoTape(size_type currentIndex, size_type newBits) noexcept {
    static constexpr size_type bitTotal{index * 64ull};
    parser->structural_indexes[0 + (currentIndex * 8) + parser->n_structural_indexes] =
        
        static_cast<uint32_t>(trailing_zeroes(newBits) + bitTotal +
                              stringIndex);
    newBits = clear_lowest_bit(newBits);
    parser->structural_indexes[1 + (currentIndex * 8) + parser->n_structural_indexes] =
        
        static_cast<uint32_t>(trailing_zeroes(newBits) + bitTotal +
                              stringIndex);
    newBits = clear_lowest_bit(newBits);
    parser->structural_indexes[2 + (currentIndex * 8) + parser->n_structural_indexes] =
        
        static_cast<uint32_t>(trailing_zeroes(newBits) + bitTotal +
                              stringIndex);
    newBits = clear_lowest_bit(newBits);
    parser->structural_indexes[3 + (currentIndex * 8) + parser->n_structural_indexes] =
        
        static_cast<uint32_t>(trailing_zeroes(newBits) + bitTotal +
                              stringIndex);
    newBits = clear_lowest_bit(newBits);
    parser->structural_indexes[4 + (currentIndex * 8) + parser->n_structural_indexes] =
        
        static_cast<uint32_t>(trailing_zeroes(newBits) + bitTotal +
                              stringIndex);
    newBits = clear_lowest_bit(newBits);
    parser->structural_indexes[5 + (currentIndex * 8) + parser->n_structural_indexes] =
        
        static_cast<uint32_t>(trailing_zeroes(newBits) + bitTotal +
                              stringIndex);
    newBits = clear_lowest_bit(newBits);
    parser->structural_indexes[6 + (currentIndex * 8) + parser->n_structural_indexes] =
        
        static_cast<uint32_t>(trailing_zeroes(newBits) + bitTotal +
                              stringIndex);
    newBits = clear_lowest_bit(newBits);
    parser->structural_indexes[7 + (currentIndex * 8) + parser->n_structural_indexes] =
        
        static_cast<uint32_t>(trailing_zeroes(newBits) + bitTotal +
                              stringIndex);
    newBits = clear_lowest_bit(newBits);
    return newBits;
  }

  template <bool collectAligned>
  simdjson2_really_inline void
  collectStringValues(const uint8_t *values,
                      simd8<uint8_t> (&newPtr)[8]) noexcept {
    if constexpr (collectAligned) {
      newPtr[0] = simd8<uint8_t>::load(values + (32 * 0));
      newPtr[1] = simd8<uint8_t>::load(values + (32 * 1));
      newPtr[2] = simd8<uint8_t>::load(values + (32 * 2));
      newPtr[3] = simd8<uint8_t>::load(values + (32 * 3));
      newPtr[4] = simd8<uint8_t>::load(values + (32 * 4));
      newPtr[5] = simd8<uint8_t>::load(values + (32 * 5));
      newPtr[6] = simd8<uint8_t>::load(values + (32 * 6));
      newPtr[7] = simd8<uint8_t>::load(values + (32 * 7));
    } else {
      newPtr[0] = simd8<uint8_t>::load(values + (32 * 0));
      newPtr[1] = simd8<uint8_t>::load(values + (32 * 1));
      newPtr[2] = simd8<uint8_t>::load(values + (32 * 2));
      newPtr[3] = simd8<uint8_t>::load(values + (32 * 3));
      newPtr[4] = simd8<uint8_t>::load(values + (32 * 4));
      newPtr[5] = simd8<uint8_t>::load(values + (32 * 5));
      newPtr[6] = simd8<uint8_t>::load(values + (32 * 6));
      newPtr[7] = simd8<uint8_t>::load(values + (32 * 7));
    }
    // prefetchStringValues(values + 256);
  }

  template <bool collectAligned, bool minified>
  simdjson2_really_inline simd_int_t_holder
  getRawIndices(const uint8_t *values) noexcept {
    simd8<uint8_t> newPtr[8];
    collectStringValues<collectAligned>(values, newPtr);
    return collectIndices(newPtr);
  }

  template <bool collectAligned, bool minified>
  simdjson2_really_inline void
  generateStructurals(const uint8_t *values, simd8<uint8_t> &escaped,
                      simd8<uint8_t> &nextIsEscaped,
                      simd_int_t_holder &rawStructurals) noexcept {
    rawStructurals = getRawIndices<collectAligned, minified>(values);
    collectStructurals<minified>(escaped, nextIsEscaped, rawStructurals);
    rawStructurals.op.store(newBits);
    addTapeValues();
    stringIndex += 256;
  }

  template <size_type index = 0>
  simdjson2_really_inline void addTapeValues() noexcept {
    if constexpr (index < 4) {
      if ((!newBits[index])) {
        return addTapeValues<index + 1>();
      }
      auto cnt = count_ones(newBits[index]);
      size_type rollsAmount =
          static_cast<size_type>(std::ceil(static_cast<double>(cnt) / 8.0f));
      for (size_type y = 0; y < rollsAmount; ++y) {
        newBits[index] = rollValuesIntoTape<index>(y, newBits[index]);
      }
      parser->n_structural_indexes += std::ceil(static_cast<double>(cnt));
      return addTapeValues<index + 1>();
    }
  }

  simdjson2_really_inline void
  collectEscaped(simd8<uint8_t> &escaped, simd8<uint8_t> &nextIsEscaped,
                 simd_int_t_holder &rawStructurals) noexcept {
    simd8<uint8_t> oddBitsVal{simd8<uint8_t>::splat(0xAA)};
    simd8<uint8_t> potentialEscape =
        rawStructurals.backslashes.bit_andnot(nextIsEscaped);
    simd8<uint8_t> maybeEscaped;
    opShl(1, potentialEscape, maybeEscaped);
    simd8<uint8_t> maybeEscapedAndOddBits = (maybeEscaped | oddBitsVal);
    simd8<uint8_t> evenSeriesCodesAndOddBits =
        opSub(maybeEscapedAndOddBits, potentialEscape);
    simd8<uint8_t> escapeAndTerminalCode =
        (evenSeriesCodesAndOddBits ^ oddBitsVal);
    escaped =
        escapeAndTerminalCode ^ (rawStructurals.backslashes | nextIsEscaped);
    nextIsEscaped.set_lsb(
        (escapeAndTerminalCode & rawStructurals.backslashes).get_msb());
  }

  simdjson2_really_inline void
  collectEmptyEscaped(simd8<uint8_t> &escaped,
                      simd8<uint8_t> &nextIsEscaped) noexcept {
    auto escapedNew = nextIsEscaped;
    nextIsEscaped = simd8<uint8_t>{};
    escaped = escapedNew;
  }

  uint64_t reverseBits(uint64_t value) {
    value = ((value >> 1) & 0x5555555555555555ULL) |
            ((value & 0x5555555555555555ULL) << 1);
    value = ((value >> 2) & 0x3333333333333333ULL) |
            ((value & 0x3333333333333333ULL) << 2);
    value = ((value >> 4) & 0x0F0F0F0F0F0F0F0FULL) |
            ((value & 0x0F0F0F0F0F0F0F0FULL) << 4);
    value = ((value >> 8) & 0x00FF00FF00FF00FFULL) |
            ((value & 0x00FF00FF00FF00FFULL) << 8);
    value = ((value >> 16) & 0x0000FFFF0000FFFFULL) |
            ((value & 0x0000FFFF0000FFFFULL) << 16);
    value = (value >> 32) | (value << 32);
    return value;
  }

  simdjson2_really_inline void
  collectEscapedCharacters(simd8<uint8_t> &escaped,
                           simd8<uint8_t> &nextIsEscaped,
                           simd_int_t_holder &rawStructurals) noexcept {
    return rawStructurals.backslashes.test()
               ? collectEscaped(escaped, nextIsEscaped, rawStructurals)
               : collectEmptyEscaped(escaped, nextIsEscaped);
  }

  template <bool minified>
  simdjson2_really_inline void
  collectStructurals(simd8<uint8_t> &escaped, simd8<uint8_t> &nextIsEscaped,
                     simd_int_t_holder &rawStructurals) noexcept {
    collectEscapedCharacters(escaped, nextIsEscaped, rawStructurals);
    rawStructurals.quotes = rawStructurals.quotes.bit_andnot(escaped);
    simd8<uint8_t> inString = opClMul(rawStructurals.quotes, prevInString);
    simd8<uint8_t> stringTail = inString ^ rawStructurals.quotes;
    simd8<uint8_t> scalar = ~(rawStructurals.op | rawStructurals.whitespace);
    simd8<uint8_t> nonQuoteScalar = scalar.bit_andnot(rawStructurals.quotes);
    simd8<uint8_t> followsNonQuoteScalar = opFollows(nonQuoteScalar, overflow);
    simd8<uint8_t> potentialScalarStart =
        scalar.bit_andnot(followsNonQuoteScalar);
    simd8<uint8_t> porentialStructuralStart =
        rawStructurals.op | potentialScalarStart;
    rawStructurals.op = porentialStructuralStart.bit_andnot(stringTail);
  }
};

} // namespace stage1
} // unnamed namespace
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

// Clear CUSTOM_BIT_INDEXER so other implementations can set it if they need to.
#undef SIMDJSON2_GENERIC_JSON_STRUCTURAL_INDEXER_CUSTOM_BIT_INDEXER

#endif // SIMDJSON2_SRC_GENERIC_STAGE1_JSON_STRUCTURAL_INDEXER_H
