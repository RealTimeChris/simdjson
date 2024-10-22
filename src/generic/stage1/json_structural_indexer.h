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

template<size_t step_size>
class string_block_reader {
public:
  simdjson2_really_inline void reset(const uint8_t *stringViewNew,
                                     uint64_t lengthNew) noexcept {
    lengthMinusStep = lengthNew < step_size ? 0 : lengthNew - step_size;
    inString = stringViewNew;
    length = lengthNew;
    index = 0;
  }

  simdjson2_really_inline uint64_t getRemainder(uint8_t *dest) const noexcept {
    if ((length == index)) {
      return 0;
    }
    std::fill_n(dest, step_size, static_cast<char>(0x20));
    std::copy_n(inString + index, length - index, dest);
    return length - index;
  }

  simdjson2_really_inline const uint8_t *fullBlock() noexcept {
    const uint8_t *newPtr = inString + index;
    index += step_size;
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
    currentParseBuffer = static_cast<const uint8_t *>(buf);
    currentBufferLength = len;
    resetImpl();
    return error_code::SUCCESS;
  }

  simdjson2_really_inline ~json_structural_indexer() noexcept {}

protected:
  const uint8_t* currentParseBuffer{};
  size_t currentBufferLength{};
  string_block_reader<sizeof(simd8<uint8_t>) == 32 ? 256 : 128>
      stringBlockReader{};
  dom_parser_implementation *parser{};
  alignas(sizeof(simd8<uint8_t>))
      size_type newBits[sizeof(simd8<uint8_t>) == 32 ? 4 : 2]{};
  alignas(sizeof(simd8<uint8_t>)) uint8_t block[256]{};
  size_type stringIndex{};
  int64_t prevInString{};
  bool overflow{};

  template <bool minified = false>
  simdjson2_really_inline void resetImpl() noexcept {
    stringBlockReader.reset(currentParseBuffer, currentBufferLength);
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
    if constexpr (sizeof(simd8<uint8_t>) == 16) {
      stringIndex += 128;
    } else {
      stringIndex += 256;
    }
  }

  template <size_type index = 0>
  simdjson2_really_inline void addTapeValues() noexcept {
    if constexpr (index < (sizeof(simd8<uint8_t>) == 32 ? 4 : 2)) {
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
    simd8<uint8_t> maybeEscapedAndOddBits = opOr(maybeEscaped, oddBitsVal);
    simd8<uint8_t> evenSeriesCodesAndOddBits =
        opSub(maybeEscapedAndOddBits, potentialEscape);
    simd8<uint8_t> escapeAndTerminalCode =
        (evenSeriesCodesAndOddBits ^ oddBitsVal);
    escaped = opXor(escapeAndTerminalCode,
                    opOr(rawStructurals.backslashes, nextIsEscaped));
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
    simd8<uint8_t> stringTail = opXor(inString, rawStructurals.quotes);
    simd8<uint8_t> scalar =
        opNot(opOr(rawStructurals.op, rawStructurals.whitespace));
    simd8<uint8_t> nonQuoteScalar = scalar.bit_andnot(rawStructurals.quotes);
    simd8<uint8_t> followsNonQuoteScalar = opFollows(nonQuoteScalar, overflow);
    simd8<uint8_t> potentialScalarStart =
        scalar.bit_andnot(followsNonQuoteScalar);
    simd8<uint8_t> porentialStructuralStart =
        opOr(rawStructurals.op, potentialScalarStart);
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
