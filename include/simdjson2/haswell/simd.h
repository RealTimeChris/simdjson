#ifndef SIMDJSON2_HASWELL_SIMD_H
#define SIMDJSON2_HASWELL_SIMD_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#include "simdjson2/haswell/base.h"
#include "simdjson2/haswell/bitmanipulation.h"
#include "simdjson2/haswell/intrinsics.h"
#include "simdjson2/internal/simdprune_tables.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace haswell {
namespace {
namespace simd {

// Forward-declared so they can be used by splat and friends.
template <typename Child> struct base {
  __m256i value;

  // Zero constructor
  simdjson2_really_inline base() : value{__m256i()} {}

  // Conversion from SIMD register
  simdjson2_really_inline base(const __m256i _value) : value(_value) {}

  // Conversion to SIMD register
  simdjson2_really_inline operator const __m256i &() const {
    return this->value;
  }
  simdjson2_really_inline operator __m256i &() { return this->value; }

  // Bit operations
  simdjson2_really_inline Child operator|(const Child other) const {
    return _mm256_or_si256(*this, other);
  }
  simdjson2_really_inline Child operator&(const Child other) const {
    return _mm256_and_si256(*this, other);
  }
  simdjson2_really_inline Child operator^(const Child other) const {
    return _mm256_xor_si256(*this, other);
  }
  simdjson2_really_inline Child bit_andnot(const Child other) const {
    return _mm256_andnot_si256(other, *this);
  }
  simdjson2_really_inline Child &operator|=(const Child other) {
    auto this_cast = static_cast<Child *>(this);
    *this_cast = *this_cast | other;
    return *this_cast;
  }
  simdjson2_really_inline Child &operator&=(const Child other) {
    auto this_cast = static_cast<Child *>(this);
    *this_cast = *this_cast & other;
    return *this_cast;
  }
  simdjson2_really_inline Child &operator^=(const Child other) {
    auto this_cast = static_cast<Child *>(this);
    *this_cast = *this_cast ^ other;
    return *this_cast;
  }
};

simdjson2_really_inline static uint64_t
prefixXor(uint64_t prevInString) noexcept {
  prevInString ^= prevInString << 1;
  prevInString ^= prevInString << 2;
  prevInString ^= prevInString << 4;
  prevInString ^= prevInString << 8;
  prevInString ^= prevInString << 16;
  prevInString ^= prevInString << 32;
  return prevInString;
}

// Override to distinguish from bool version
simdjson2_really_inline __m256i opNot(const __m256i&value) {
  return _mm256_xor_si256(value, _mm256_set1_epi64x(0xFFFFFFFFFFFFFFFFll));
  ;
}

simdjson2_really_inline __m256i opOr(const __m256i &value,
                                     const __m256i &other) {
  return _mm256_or_si256(value, other);
}

simdjson2_really_inline __m256i opAnd(const __m256i &value,
                                      const __m256i &other) {
  return _mm256_and_si256(value, other);
}

simdjson2_really_inline __m256i opXor(const __m256i &value,
                                      const __m256i &other) {
  return _mm256_xor_si256(value, other);
}

// Forward-declared so they can be used by splat and friends.
template <typename T> struct simd8;

template <typename T, typename Mask = simd8<bool>>
struct base8 : base<simd8<T>> {
  typedef uint32_t bitmask_t;
  typedef uint64_t bitmask2_t;

  simdjson2_really_inline base8() : base<simd8<T>>() {}
  simdjson2_really_inline base8(const __m256i _value)
      : base<simd8<T>>(_value) {}

  friend simdjson2_really_inline Mask operator==(const simd8<T>& lhs,
                                                 const simd8<T>& rhs) {
    return _mm256_cmpeq_epi8(lhs, rhs);
  }

  static const int SIZE = sizeof(base<T>::value);

  template <int N = 1>
  simdjson2_really_inline __m256i prev(const simd8<T>& prev_chunk) const {
    return _mm256_alignr_epi8(
        *this, _mm256_permute2x128_si256(prev_chunk, *this, 0x21), 16 - N);
  }
};

// SIMD byte mask type (returned by things like eq and gt)
template <> struct simd8<bool> : base8<bool> {
  static simdjson2_really_inline simd8<bool> splat(bool _value) {
    return _mm256_set1_epi8(uint8_t(-(!!_value)));
  }

  simdjson2_really_inline simd8() : base8() {}
  simdjson2_really_inline simd8(const __m256i _value) : base8<bool>(_value) {}
  // Splat constructor
  simdjson2_really_inline simd8(bool _value) : base8<bool>(splat(_value)) {}

  simdjson2_really_inline int to_bitmask() const {
    return _mm256_movemask_epi8(*this);
  }
  simdjson2_really_inline bool any() const {
    return !_mm256_testz_si256(*this, *this);
  }
  simdjson2_really_inline simd8<bool> operator~() const { return *this ^ true; }
};

template <typename T> struct base8_numeric : base8<T> {
  static simdjson2_really_inline __m256i splat(T _value) {
    return _mm256_set1_epi8(_value);
  }
  static simdjson2_really_inline __m256i zero() {
    return _mm256_setzero_si256();
  }
  static simdjson2_really_inline __m256i load(const T values[32]) {
    return _mm256_loadu_si256(reinterpret_cast<const __m256i *>(values));
  }
  // Repeat 16 values as many times as necessary (usually for lookup tables)
  static simdjson2_really_inline __m256i
  repeat_16(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10,
            T v11, T v12, T v13, T v14, T v15) {
    return simd8<T>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,
                    v14, v15, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,
                    v12, v13, v14, v15);
  }

  static simdjson2_really_inline __m256i load_a(const uint64_t *values) {
    return _mm256_load_si256(reinterpret_cast<const __m256i *>(values));
  }

  static simdjson2_really_inline __m256i load(const uint32_t *values) {
    return _mm256_loadu_si256(reinterpret_cast<const __m256i *>(values));
  }

  simdjson2_really_inline void store(uint64_t *ptr) const {
    _mm256_store_si256(reinterpret_cast<__m256i *>(ptr), *this);
  }

  simdjson2_really_inline base8_numeric() : base8<T>() {}
  simdjson2_really_inline base8_numeric(const __m256i _value)
      : base8<T>(_value) {}

  // Store to array
  simdjson2_really_inline void store(T dst[32]) const {
    return _mm256_storeu_si256(reinterpret_cast<__m256i *>(dst), *this);
  }

  // Addition/subtraction are the same for signed and unsigned
  simdjson2_really_inline __m256i operator+(const simd8<T>& other) const {
    return _mm256_add_epi8(*this, other);
  }
  simdjson2_really_inline __m256i operator-(const simd8<T>& other) const {
    return _mm256_sub_epi8(*this, other);
  }
  simdjson2_really_inline __m256i &operator+=(const simd8<T>& other) {
    *this = *this + other;
    return *static_cast<simd8<T> *>(this);
  }
  simdjson2_really_inline __m256i &operator-=(const simd8<T>& other) {
    *this = *this - other;
    return *static_cast<simd8<T> *>(this);
  }

  // Override to distinguish from bool version
  simdjson2_really_inline __m256i operator~() const {
    return _mm256_xor_si256(*this, _mm256_set1_epi64x(0xFFFFFFFFFFFFFFFFll));
    ;
  }

  // Perform a lookup assuming the value is between 0 and 16 (undefined behavior
  // for out of range values)
  template <typename L>
  simdjson2_really_inline simd8<L> lookup_16(simd8<L> lookup_table) const {
    return _mm256_shuffle_epi8(lookup_table, *this);
  }

  // Copies to 'output" all bytes corresponding to a 0 in the mask (interpreted
  // as a bitset). Passing a 0 value for mask would be equivalent to writing out
  // every byte to output. Only the first 32 - count_ones(mask) bytes of the
  // result are significant but 32 bytes get written. Design consideration: it
  // seems like a function with the signature simd8<L> compress(uint32_t mask)
  // would be sensible, but the AVX ISA makes this kind of approach difficult.
  template <typename L>
  simdjson2_really_inline void compress(uint32_t mask, L *output) const {
    using internal::BitsSetTable256mul2;
    using internal::pshufb_combine_table;
    using internal::thintable_epi8;
    // this particular implementation was inspired by work done by @animetosho
    // we do it in four steps, first 8 bytes and then second 8 bytes...
    uint8_t mask1 = uint8_t(mask);       // least significant 8 bits
    uint8_t mask2 = uint8_t(mask >> 8);  // second least significant 8 bits
    uint8_t mask3 = uint8_t(mask >> 16); // ...
    uint8_t mask4 = uint8_t(mask >> 24); // ...
    // next line just loads the 64-bit values thintable_epi8[mask1] and
    // thintable_epi8[mask2] into a 128-bit register, using only
    // two instructions on most compilers.
    __m256i shufmask =
        _mm256_set_epi64x(thintable_epi8[mask4], thintable_epi8[mask3],
                          thintable_epi8[mask2], thintable_epi8[mask1]);
    // we increment by 0x08 the second half of the mask and so forth
    shufmask = _mm256_add_epi8(
        shufmask, _mm256_set_epi32(0x18181818, 0x18181818, 0x10101010,
                                   0x10101010, 0x08080808, 0x08080808, 0, 0));
    // this is the version "nearly pruned"
    __m256i pruned = _mm256_shuffle_epi8(*this, shufmask);
    // we still need to put the  pieces back together.
    // we compute the popcount of the first words:
    int pop1 = BitsSetTable256mul2[mask1];
    int pop3 = BitsSetTable256mul2[mask3];

    // then load the corresponding mask
    // could be done with _mm256_loadu2_m128i but many standard libraries omit
    // this intrinsic.
    __m256i v256 = _mm256_castsi128_si256(_mm_loadu_si128(
        reinterpret_cast<const __m128i *>(pshufb_combine_table + pop1 * 8)));
    __m256i compactmask = _mm256_insertf128_si256(
        v256,
        _mm_loadu_si128(
            reinterpret_cast<const __m128i *>(pshufb_combine_table + pop3 * 8)),
        1);
    __m256i almostthere = _mm256_shuffle_epi8(pruned, compactmask);
    // We just need to write out the result.
    // This is the tricky bit that is hard to do
    // if we want to return a SIMD register, since there
    // is no single-instruction approach to recombine
    // the two 128-bit lanes with an offset.
    __m128i v128;
    v128 = _mm256_castsi256_si128(almostthere);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(output), v128);
    v128 = _mm256_extractf128_si256(almostthere, 1);
    _mm_storeu_si128(
        reinterpret_cast<__m128i *>(output + 16 - count_ones(mask & 0xFFFF)),
        v128);
  }

  template <typename L>
  simdjson2_really_inline simd8<L>
  lookup_16(L replace0, L replace1, L replace2, L replace3, L replace4,
            L replace5, L replace6, L replace7, L replace8, L replace9,
            L replace10, L replace11, L replace12, L replace13, L replace14,
            L replace15) const {
    return lookup_16(simd8<L>::repeat_16(
        replace0, replace1, replace2, replace3, replace4, replace5, replace6,
        replace7, replace8, replace9, replace10, replace11, replace12,
        replace13, replace14, replace15));
  }
};

// Signed bytes
template <> struct simd8<int8_t> : base8_numeric<int8_t> {
  simdjson2_really_inline simd8() : base8_numeric<int8_t>() {}
  simdjson2_really_inline simd8(const __m256i _value)
      : base8_numeric<int8_t>(_value) {}
  // Splat constructor
  simdjson2_really_inline simd8(int8_t _value) : simd8(splat(_value)) {}
  // Array constructor
  simdjson2_really_inline simd8(const int8_t values[32])
      : simd8(load(values)) {}
  // Member-by-member initialization
  simdjson2_really_inline
  simd8(int8_t v0, int8_t v1, int8_t v2, int8_t v3, int8_t v4, int8_t v5,
        int8_t v6, int8_t v7, int8_t v8, int8_t v9, int8_t v10, int8_t v11,
        int8_t v12, int8_t v13, int8_t v14, int8_t v15, int8_t v16, int8_t v17,
        int8_t v18, int8_t v19, int8_t v20, int8_t v21, int8_t v22, int8_t v23,
        int8_t v24, int8_t v25, int8_t v26, int8_t v27, int8_t v28, int8_t v29,
        int8_t v30, int8_t v31)
      : simd8(_mm256_setr_epi8(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,
                               v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,
                               v22, v23, v24, v25, v26, v27, v28, v29, v30,
                               v31)) {}
  // Repeat 16 values as many times as necessary (usually for lookup tables)
  simdjson2_really_inline static simd8<int8_t>
  repeat_16(int8_t v0, int8_t v1, int8_t v2, int8_t v3, int8_t v4, int8_t v5,
            int8_t v6, int8_t v7, int8_t v8, int8_t v9, int8_t v10, int8_t v11,
            int8_t v12, int8_t v13, int8_t v14, int8_t v15) {
    return simd8<int8_t>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12,
                         v13, v14, v15, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9,
                         v10, v11, v12, v13, v14, v15);
  }

  // Order-sensitive comparisons
  simdjson2_really_inline simd8<int8_t>
  max_val(const simd8<int8_t> other) const {
    return _mm256_max_epi8(*this, other);
  }
  simdjson2_really_inline simd8<int8_t>
  min_val(const simd8<int8_t> other) const {
    return _mm256_min_epi8(*this, other);
  }
  simdjson2_really_inline simd8<bool>
  operator>(const simd8<int8_t> other) const {
    return _mm256_cmpgt_epi8(*this, other);
  }
  simdjson2_really_inline simd8<bool>
  operator<(const simd8<int8_t> other) const {
    return _mm256_cmpgt_epi8(other, *this);
  }
};

// Unsigned bytes
template <> struct simd8<uint8_t> : base8_numeric<uint8_t> {
  simdjson2_really_inline simd8() : base8_numeric<uint8_t>() {}
  simdjson2_really_inline simd8(const __m256i _value)
      : base8_numeric<uint8_t>(_value) {}
  // Splat constructor
  simdjson2_really_inline simd8(uint8_t _value) : simd8(splat(_value)) {}
  // Array constructor
  simdjson2_really_inline simd8(const uint8_t values[32])
      : simd8(base8_numeric<uint8_t>::load(values)) {}
  // Member-by-member initialization
  simdjson2_really_inline
  simd8(uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5,
        uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10,
        uint8_t v11, uint8_t v12, uint8_t v13, uint8_t v14, uint8_t v15,
        uint8_t v16, uint8_t v17, uint8_t v18, uint8_t v19, uint8_t v20,
        uint8_t v21, uint8_t v22, uint8_t v23, uint8_t v24, uint8_t v25,
        uint8_t v26, uint8_t v27, uint8_t v28, uint8_t v29, uint8_t v30,
        uint8_t v31)
      : simd8(_mm256_setr_epi8(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,
                               v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,
                               v22, v23, v24, v25, v26, v27, v28, v29, v30,
                               v31)) {}
  // Repeat 16 values as many times as necessary (usually for lookup tables)
  simdjson2_really_inline static simd8<uint8_t>
  repeat_16(uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4,
            uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9,
            uint8_t v10, uint8_t v11, uint8_t v12, uint8_t v13, uint8_t v14,
            uint8_t v15) {
    return simd8<uint8_t>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12,
                          v13, v14, v15, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9,
                          v10, v11, v12, v13, v14, v15);
  }

  // Saturated math
  simdjson2_really_inline simd8<uint8_t>
  saturating_add(const simd8<uint8_t> other) const {
    return _mm256_adds_epu8(*this, other);
  }
  simdjson2_really_inline simd8<uint8_t>
  saturating_sub(const simd8<uint8_t> other) const {
    return _mm256_subs_epu8(*this, other);
  }

  simdjson2_really_inline uint64_t lteq(const uint8_t m) const {
    return (*this <= simd8<uint8_t>{simd8<uint8_t>::splat(m)}).to_bitmask();
  }

  simdjson2_really_inline bool test() const {
    return !_mm256_testz_si256(*this, *this);
  }

  simdjson2_really_inline simd8<uint8_t> eq(const simd8<uint8_t> &m) const {
    return _mm256_cmpeq_epi8(*this, m);
  }

  simdjson2_really_inline uint64_t eq(const uint8_t m) const {
    return (*this == simd8<uint8_t>{simd8<uint8_t>::splat(m)}).to_bitmask();
  }

  simdjson2_really_inline void set_lsb(bool valueNew) noexcept {
    const simd8<uint8_t> mask{0x01u, '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                              '\0',  '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                              '\0',  '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                              '\0',  '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
    *this = valueNew ? _mm256_or_si256(*this, mask)
                     : _mm256_andnot_si256(mask, *this);
  }

  simdjson2_really_inline bool get_msb() const noexcept {
    auto result = _mm256_and_si256(
        *this, _mm256_set_epi64x(0x8000000000000000ll, 0x00ll, 0x00ll, 0x00ll));
    return !_mm256_testz_si256(result, result);
  }

  // Order-specific operations
  simdjson2_really_inline simd8<uint8_t>
  max_val(const simd8<uint8_t> other) const {
    return _mm256_max_epu8(*this, other);
  }
  simdjson2_really_inline simd8<uint8_t>
  min_val(const simd8<uint8_t> other) const {
    return _mm256_min_epu8(other, *this);
  }
  // Same as >, but only guarantees true is nonzero (< guarantees true = -1)
  simdjson2_really_inline simd8<uint8_t>
  gt_bits(const simd8<uint8_t> other) const {
    return this->saturating_sub(other);
  }
  // Same as <, but only guarantees true is nonzero (< guarantees true = -1)
  simdjson2_really_inline simd8<uint8_t>
  lt_bits(const simd8<uint8_t> other) const {
    return other.saturating_sub(*this);
  }
  simdjson2_really_inline simd8<bool>
  operator<=(const simd8<uint8_t> other) const {
    return other.max_val(*this) == other;
  }
  simdjson2_really_inline simd8<bool>
  operator>=(const simd8<uint8_t> other) const {
    return other.min_val(*this) == other;
  }
  simdjson2_really_inline simd8<bool>
  operator>(const simd8<uint8_t> other) const {
    return this->gt_bits(other).any_bits_set();
  }
  simdjson2_really_inline simd8<bool>
  operator<(const simd8<uint8_t> other) const {
    return this->lt_bits(other).any_bits_set();
  }

  // Bit-specific operations
  simdjson2_really_inline simd8<bool> bits_not_set() const {
    return *this == uint8_t(0);
  }
  simdjson2_really_inline simd8<bool> bits_not_set(simd8<uint8_t> bits) const {
    return (*this & bits).bits_not_set();
  }
  simdjson2_really_inline simd8<bool> any_bits_set() const {
    return ~this->bits_not_set();
  }
  simdjson2_really_inline simd8<bool> any_bits_set(simd8<uint8_t> bits) const {
    return ~this->bits_not_set(bits);
  }
  simdjson2_really_inline bool is_ascii() const {
    return _mm256_movemask_epi8(*this) == 0;
  }
  simdjson2_really_inline bool bits_not_set_anywhere() const {
    return _mm256_testz_si256(*this, *this);
  }
  simdjson2_really_inline bool any_bits_set_anywhere() const {
    return !bits_not_set_anywhere();
  }
  simdjson2_really_inline bool
  bits_not_set_anywhere(simd8<uint8_t> bits) const {
    return _mm256_testz_si256(*this, bits);
  }
  simdjson2_really_inline bool
  any_bits_set_anywhere(simd8<uint8_t> bits) const {
    return !bits_not_set_anywhere(bits);
  }
  template <int N> simdjson2_really_inline simd8<uint8_t> shr() const {
    return simd8<uint8_t>(_mm256_srli_epi16(*this, N)) & uint8_t(0xFFu >> N);
  }
  template <int N> simdjson2_really_inline simd8<uint8_t> shl() const {
    return simd8<uint8_t>(_mm256_slli_epi16(*this, N)) & uint8_t(0xFFu << N);
  }
  // Get one of the bits and make a bitmask out of it.
  // e.g. value.get_bit<7>() gets the high bit
  template <int N> simdjson2_really_inline int get_bit() const {
    return _mm256_movemask_epi8(_mm256_slli_epi16(*this, 7 - N));
  }
};

template <typename T> struct simd8x64 {
  static constexpr int NUM_CHUNKS = 64 / sizeof(simd8<T>);
  static_assert(NUM_CHUNKS == 2,
                "Haswell kernel should use two registers per 64-byte block.");
  const simd8<T> chunks[NUM_CHUNKS];

  simd8x64(const simd8x64<T> &o) = delete; // no copy allowed
  simd8x64<T> &
  operator=(const simd8<T>& other) = delete; // no assignment allowed
  simd8x64() = delete;                       // no default constructor allowed

  simdjson2_really_inline simd8x64(const simd8<T>& chunk0, const simd8<T>& chunk1)
      : chunks{chunk0, chunk1} {}
  simdjson2_really_inline simd8x64(const T ptr[64])
      : chunks{simd8<T>::load(ptr), simd8<T>::load(ptr + 32)} {}

  simdjson2_really_inline uint64_t compress(uint64_t mask, T *output) const {
    uint32_t mask1 = uint32_t(mask);
    uint32_t mask2 = uint32_t(mask >> 32);
    this->chunks[0].compress(mask1, output);
    this->chunks[1].compress(mask2, output + 32 - count_ones(mask1));
    return 64 - count_ones(mask);
  }

  simdjson2_really_inline void store(T ptr[64]) const {
    this->chunks[0].store(ptr + sizeof(simd8<T>) * 0);
    this->chunks[1].store(ptr + sizeof(simd8<T>) * 1);
  }

  simdjson2_really_inline uint64_t to_bitmask() const {
    uint64_t r_lo = uint32_t(this->chunks[0].to_bitmask());
    uint64_t r_hi = this->chunks[1].to_bitmask();
    return r_lo | (r_hi << 32);
  }

  simdjson2_really_inline __m256i reduce_or() const {
    return opOr(this->chunks[0], this->chunks[1]);
  }

  simdjson2_really_inline simd8x64<T> bit_or(const T m) const {
    const simd8<T>& mask = simd8<T>::splat(m);
    return simd8x64<T>(opOr(this->chunks[0], mask),
                       opOr(this->chunks[1], mask));
  }

  simdjson2_really_inline uint64_t eq(const T m) const {
    const simd8<T>& mask = simd8<T>::splat(m);
    return simd8x64<bool>(this->chunks[0] == mask, this->chunks[1] == mask)
        .to_bitmask();
  }

  simdjson2_really_inline uint64_t eq(const simd8x64<uint8_t> &other) const {
    return simd8x64<bool>(this->chunks[0] == other.chunks[0],
                          this->chunks[1] == other.chunks[1])
        .to_bitmask();
  }

  simdjson2_really_inline uint64_t lteq(const T m) const {
    const simd8<T>& mask = simd8<T>::splat(m);
    return simd8x64<bool>(this->chunks[0] <= mask, this->chunks[1] <= mask)
        .to_bitmask();
  }
}; // struct simd8x64<T>

template <typename simd_int_t01>
simdjson2_really_inline simd8<uint8_t> opClMul(simd_int_t01 &&value,
                                               int64_t &prevInString) noexcept {
  alignas(32) uint64_t values[4];
  value.store(values);
  values[0] = prefixXor(values[0]) ^ prevInString;
  prevInString = static_cast<int64_t>(values[0]) >> 63;
  values[1] = prefixXor(values[1]) ^ prevInString;
  prevInString = static_cast<int64_t>(values[1]) >> 63;
  values[2] = prefixXor(values[2]) ^ prevInString;
  prevInString = static_cast<int64_t>(values[2]) >> 63;
  values[3] = prefixXor(values[3]) ^ prevInString;
  prevInString = static_cast<int64_t>(values[3]) >> 63;
  return simd8<uint8_t>::load_a(values);
}

template <typename simd_int_t01, typename simd_int_t02>
simdjson2_really_inline simd8<uint8_t> opSub(simd_int_t01 &&value,
                                             simd_int_t02 &&other) noexcept {
  alignas(32) uint64_t values[4 * 2];
  value.store(values);
  other.store(values + 4);
  bool carryInNew{};
  values[4] = values[0] - values[4] - static_cast<uint64_t>(carryInNew);
  carryInNew = values[4] > values[0];
  values[1 + 4] = values[1] - values[1 + 4] - static_cast<uint64_t>(carryInNew);
  carryInNew = values[1 + 4] > values[1];
  if constexpr (4 > 2) {
    values[2 + 4] =
        values[2] - values[2 + 4] - static_cast<uint64_t>(carryInNew);
    carryInNew = values[2 + 4] > values[2];
    values[3 + 4] =
        values[3] - values[3 + 4] - static_cast<uint64_t>(carryInNew);
    carryInNew = values[3 + 4] > values[3];
  }
  return simd8<uint8_t>::load_a(values + 4);
}

#define opShl(amount, value, result)                                           \
  {                                                                            \
    alignas(32) uint64_t values[4 * 2];                                        \
    value.store(values);                                                       \
    static constexpr uint64_t shiftAmount{64 - amount};                        \
    values[4] = values[0] << amount;                                           \
    values[1 + 4] = values[1] << amount | values[1 - 1] >> (shiftAmount);      \
    values[2 + 4] = values[2] << amount | values[2 - 1] >> (shiftAmount);      \
    values[3 + 4] = values[3] << amount | values[3 - 1] >> (shiftAmount);      \
    result = simd8<uint8_t>::load_a(values + 4);                               \
  }

template <typename simd_int_t01>
simdjson2_really_inline simd8<uint8_t> opFollows(simd_int_t01 &&value,
                                                 bool &overflow) noexcept {
  bool oldOverflow = overflow;
  overflow = value.get_msb();
  simd8<uint8_t> resultVal;
  opShl(1, value, resultVal);
  resultVal.set_lsb(oldOverflow);
  return resultVal;
}

struct simd_int_t_holder {
  simd8<uint8_t> backslashes;
  simd8<uint8_t> whitespace;
  simd8<uint8_t> quotes;
  simd8<uint8_t> op;
};

template <size_t size>
alignas(32) constexpr std::array<uint8_t, size> escapeableArray00{[] {
  constexpr const uint8_t values[]{0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
                                   0x00u, 0x00u, 0x00u, '\t',  '\n',  0x00u,
                                   '\\',  0x00u, 0x00u, 0x00u};
  std::array<uint8_t, size> returnValues{};
  for (uint64_t x = 0; x < size; ++x) {
    returnValues[x] = values[x % 16];
  }
  return returnValues;
}()};

template <size_t size>
alignas(32) constexpr std::array<uint8_t, size> escapeableArray01{[] {
  constexpr const uint8_t values[]{0x00u, 0x00u, '"',   0x00u, 0x00u, 0x00u,
                                   0x00u, 0x00u, '\b',  0x00u, 0x00u, 0x00u,
                                   0x0Cu, '\r',  0x00u, 0x00u};
  std::array<uint8_t, size> returnValues{};
  for (uint64_t x = 0; x < size; ++x) {
    returnValues[x] = values[x % 16];
  }
  return returnValues;
}()};

template <size_t size>
alignas(32) constexpr std::array<uint8_t, size> whitespaceArray{[] {
  constexpr const uint8_t values[]{0x20u, 0x64u, 0x64u, 0x64u, 0x11u, 0x64u,
                                   0x71u, 0x02u, 0x64u, '\t',  '\n',  0x70u,
                                   0x64u, '\r',  0x64u, 0x64u};
  std::array<uint8_t, size> returnValues{};
  for (uint64_t x = 0; x < size; ++x) {
    returnValues[x] = values[x % 16];
  }
  return returnValues;
}()};

template <size_t size>
alignas(32) constexpr std::array<uint8_t, size> opArray{[] {
  constexpr const uint8_t values[]{0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
                                   0x00u, 0x00u, 0x00u, 0x00u, ':',   '{',
                                   ',',   '}',   0x00u, 0x00u};
  std::array<uint8_t, size> returnValues{};
  for (uint64_t x = 0; x < size; ++x) {
    returnValues[x] = values[x % 16];
  }
  return returnValues;
}()};

simdjson2_really_inline simd8<uint8_t>
collectStructuralIndices(const simd8<uint8_t> *values) noexcept {
  alignas(32) uint32_t valuesNew[8];
  simd8<uint8_t> simdValues{simd8<uint8_t>::load(opArray<32>.data())};
  simd8<uint8_t> simdValue{simd8<uint8_t>::splat(static_cast<uint8_t>(0x20))};
  valuesNew[0] = _mm256_movemask_epi8(
      simd8<uint8_t>{_mm256_shuffle_epi8(simdValues, values[0])} ==
      (simdValue | values[0]));
  valuesNew[1] = _mm256_movemask_epi8(
      simd8<uint8_t>{_mm256_shuffle_epi8(simdValues, values[1])} ==
      (simdValue | values[1]));
  valuesNew[2] = _mm256_movemask_epi8(
      simd8<uint8_t>{_mm256_shuffle_epi8(simdValues, values[2])} ==
      (simdValue | values[2]));
  valuesNew[3] = _mm256_movemask_epi8(
      simd8<uint8_t>{_mm256_shuffle_epi8(simdValues, values[3])} ==
      (simdValue | values[3]));
  valuesNew[4] = _mm256_movemask_epi8(
      simd8<uint8_t>{_mm256_shuffle_epi8(simdValues, values[4])} ==
      (simdValue | values[4]));
  valuesNew[5] = _mm256_movemask_epi8(
      simd8<uint8_t>{_mm256_shuffle_epi8(simdValues, values[5])} ==
      (simdValue | values[5]));
  valuesNew[6] = _mm256_movemask_epi8(
      simd8<uint8_t>{_mm256_shuffle_epi8(simdValues, values[6])} ==
      (simdValue | values[6]));
  valuesNew[7] = _mm256_movemask_epi8(
      simd8<uint8_t>{_mm256_shuffle_epi8(simdValues, values[7])} ==
      (simdValue | values[7]));
  return simd8<uint8_t>::load(valuesNew);
}

simdjson2_really_inline simd8<uint8_t>
collectWhitespaceIndices(const simd8<uint8_t> *values) noexcept {
  alignas(32) uint32_t valuesNew[8];
  simd8<uint8_t> simdValues{simd8<uint8_t>::load(whitespaceArray<32>.data())};
  valuesNew[0] = _mm256_movemask_epi8(
      _mm256_cmpeq_epi8(_mm256_shuffle_epi8(simdValues, values[0]), values[0]));
  valuesNew[1] = _mm256_movemask_epi8(
      _mm256_cmpeq_epi8(_mm256_shuffle_epi8(simdValues, values[1]), values[1]));
  valuesNew[2] = _mm256_movemask_epi8(
      _mm256_cmpeq_epi8(_mm256_shuffle_epi8(simdValues, values[2]), values[2]));
  valuesNew[3] = _mm256_movemask_epi8(
      _mm256_cmpeq_epi8(_mm256_shuffle_epi8(simdValues, values[3]), values[3]));
  valuesNew[4] = _mm256_movemask_epi8(
      _mm256_cmpeq_epi8(_mm256_shuffle_epi8(simdValues, values[4]), values[4]));
  valuesNew[5] = _mm256_movemask_epi8(
      _mm256_cmpeq_epi8(_mm256_shuffle_epi8(simdValues, values[5]), values[5]));
  valuesNew[6] = _mm256_movemask_epi8(
      _mm256_cmpeq_epi8(_mm256_shuffle_epi8(simdValues, values[6]), values[6]));
  valuesNew[7] = _mm256_movemask_epi8(
      _mm256_cmpeq_epi8(_mm256_shuffle_epi8(simdValues, values[7]), values[7]));
  return simd8<uint8_t>::load(valuesNew);
}

template <auto c>
simdjson2_really_inline simd8<uint8_t>
collectValues(const simd8<uint8_t> *values) noexcept {
  alignas(32) uint32_t valuesNew[8];
  simd8<uint8_t> simdValue{simd8<uint8_t>::splat(c)};
  valuesNew[0] = _mm256_movemask_epi8(_mm256_cmpeq_epi8(simdValue, values[0]));
  valuesNew[1] = _mm256_movemask_epi8(_mm256_cmpeq_epi8(simdValue, values[1]));
  valuesNew[2] = _mm256_movemask_epi8(_mm256_cmpeq_epi8(simdValue, values[2]));
  valuesNew[3] = _mm256_movemask_epi8(_mm256_cmpeq_epi8(simdValue, values[3]));
  valuesNew[4] = _mm256_movemask_epi8(_mm256_cmpeq_epi8(simdValue, values[4]));
  valuesNew[5] = _mm256_movemask_epi8(_mm256_cmpeq_epi8(simdValue, values[5]));
  valuesNew[6] = _mm256_movemask_epi8(_mm256_cmpeq_epi8(simdValue, values[6]));
  valuesNew[7] = _mm256_movemask_epi8(_mm256_cmpeq_epi8(simdValue, values[7]));
  return simd8<uint8_t>::load(valuesNew);
}

simdjson2_really_inline simd_int_t_holder
collectIndices(const simd8<uint8_t> *values) noexcept {
  simd_int_t_holder returnValues;
  returnValues.op = collectStructuralIndices(values);
  returnValues.quotes = collectValues<'"'>(values);
  returnValues.whitespace = collectWhitespaceIndices(values);
  returnValues.backslashes = collectValues<'\\'>(values);
  return returnValues;
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

} // namespace simd

} // unnamed namespace
} // namespace haswell
} // namespace simdjson2

#endif // SIMDJSON2_HASWELL_SIMD_H
