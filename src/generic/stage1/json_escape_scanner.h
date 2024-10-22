#ifndef SIMDJSON2_SRC_GENERIC_STAGE1_JSON_ESCAPE_SCANNER_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_SRC_GENERIC_STAGE1_JSON_ESCAPE_SCANNER_H
#include <generic/stage1/base.h>
#include <generic/stage1/buf_block_reader.h>
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace {
namespace stage1 {

/**
 * Scans for escape characters in JSON, taking care with multiple backslashes
 * (\\n vs. \n).
 */
struct json_escape_scanner {
  /** The actual escape characters (the backslashes themselves). */
  simd::simd8<uint8_t> next_is_escaped;

  struct escaped_and_escape {
    /**
     * Mask of escaped characters.
     *
     * ```
     * \n \\n \\\n \\\\n \
     * 0100100010100101000
     *  n  \   \ n  \ \
     * ```
     */
    simd::simd8<uint8_t> escaped;
    /**
     * Mask of escape characters.
     *
     * ```
     * \n \\n \\\n \\\\n \
     * 1001000101001010001
     * \  \   \ \  \ \   \
     * ```
     */
    simd::simd8<uint8_t> escape;
  };
  alignas(32) uint64_t values[4 * 2];

  simdjson2_really_inline simd8<uint8_t>
  opSub(const simd8<uint8_t> &value, const simd8<uint8_t> &other) noexcept {
    value.store(values);
    other.store(values + 4);
    bool carryInNew{};
    values[4] = values[0] - values[4] - static_cast<uint64_t>(carryInNew);
    carryInNew = values[4] > values[0];
    values[1 + 4] =
        values[1] - values[1 + 4] - static_cast<uint64_t>(carryInNew);
    carryInNew = values[1 + 4] > values[1];
    values[2 + 4] =
        values[2] - values[2 + 4] - static_cast<uint64_t>(carryInNew);
    carryInNew = values[2 + 4] > values[2];
    values[3 + 4] =
        values[3] - values[3 + 4] - static_cast<uint64_t>(carryInNew);
    carryInNew = values[3 + 4] > values[3];
    return simd8<uint8_t>::load_a(values + 4);
  }

  /**
   * Get a mask of both escape and escaped characters (the characters following
   * a backslash).
   *
   * @param potential_escape A mask of the character that can escape others (but
   * could be escaped itself). e.g. block.eq('\\')
   */
  simdjson2_really_inline escaped_and_escape
  next(const simd::simd8<uint8_t> &backslash) noexcept {

#if !SIMDJSON2_SKIP_BACKSLASH_SHORT_CIRCUIT
    if (!backslash.test()) {
      return {next_escaped_without_backslashes(), simd::simd8<uint8_t>{}};
    }
#endif

    // |                                | Mask (shows characters instead of 1's)
    // | Depth | Instructions        |
    // |--------------------------------|----------------------------------------|-------|---------------------|
    // | string                         | `\\n_\\\n___\\\n___\\\\___\\\\__\\\`
    // |       |                     | |                                | ` even
    // odd    even   odd   odd`   |       |                     | |
    // potential_escape               | ` \  \\\    \\\    \\\\   \\\\  \\\`   |
    // 1     | 1 (backslash & ~first_is_escaped) | escape_and_terminal_code | `
    // \n \ \n   \ \n   \ \    \ \   \ \`   | 5     | 5
    // (next_escape_and_terminal_code()) | escaped                        | `\
    // \ n    \ n    \ \    \ \   \ ` X | 6     | 7 (escape_and_terminal_code ^
    // (potential_escape | first_is_escaped)) | escape                         |
    // `    \ \    \ \    \ \    \ \   \ \`   | 6     | 8
    // (escape_and_terminal_code & backslash) | first_is_escaped               |
    // `\                                 `   | 7 (*) | 9 (escape >> 63) ()
    //                                                                               (*) this is not needed until the next iteration
    simd::simd8<uint8_t> escape_and_terminal_code =
        next_escape_and_terminal_code(
            backslash.bit_andnot(this->next_is_escaped));
    simd::simd8<uint8_t> escaped =
        escape_and_terminal_code ^ (backslash | this->next_is_escaped);
    simd::simd8<uint8_t> escape = escape_and_terminal_code & backslash;
    this->next_is_escaped.set_lsb(escape.get_msb());
    return {escaped, escape};
  }

private:
  simdjson2_really_inline simd::simd8<uint8_t>
  next_escaped_without_backslashes() noexcept {
    simd::simd8<uint8_t> escaped = this->next_is_escaped;
    this->next_is_escaped = this->next_is_escaped.zero();
    return escaped;
  }

  /**
   * Returns a mask of the next escape characters (masking out escaped
   * backslashes), along with any non-backslash escape codes.
   *
   * \n \\n \\\n \\\\n returns:
   * \n \   \ \n \ \
   * 11 100 1011 10100
   *
   * You are expected to mask out the first bit yourself if the previous block
   * had a trailing escape.
   *
   * & the result with potential_escape to get just the escape characters.
   * ^ the result with (potential_escape | first_is_escaped) to get escaped
   * characters.
   */
  simdjson2_really_inline simd::simd8<uint8_t> next_escape_and_terminal_code(
      const simd::simd8<uint8_t> &potential_escape) noexcept {
    // If we were to just shift and mask out any odd bits, we'd actually get a
    // *half* right answer: any even-aligned backslash runs would be correct!
    // Odd-aligned backslash runs would be inverted (\\\ would be 010 instead of
    // 101).
    //
    // ```
    // string:              | ____\\\\_\\\\_____ |
    // maybe_escaped | ODD  |     \ \   \ \      |
    //               even-aligned ^^^  ^^^^ odd-aligned
    // ```
    //
    // Taking that into account, our basic strategy is:
    //
    // 1. Use subtraction to produce a mask with 1's for even-aligned runs and
    // 0's for
    //    odd-aligned runs.
    // 2. XOR all odd bits, which masks out the odd bits in even-aligned runs,
    // and brings IN the
    //    odd bits in odd-aligned runs.
    // 3. & with backslash to clean up any stray bits.
    // runs are set to 0, and then XORing with "odd":
    //
    // |                                | Mask (shows characters instead of 1's)
    // | Instructions        |
    // |--------------------------------|----------------------------------------|---------------------|
    // | string                         | `\\n_\\\n___\\\n___\\\\___\\\\__\\\` |
    // |                                | `    even   odd    even   odd   odd` |
    // | maybe_escaped                  | `  n  \\n    \\n    \\\_   \\\_  \\` X
    // | 1 (potential_escape << 1) | maybe_escaped_and_odd          | ` \n_ \\n
    // _ \\\n_ _ \\\__ _\\\_ \\\`   | 1 (maybe_escaped | odd) |
    // even_series_codes_and_odd      | `  n_\\\  _    n_ _\\\\ _     _    `   |
    // 1 (maybe_escaped_and_odd - potential_escape) | escape_and_terminal_code
    // | ` \n \ \n   \ \n   \ \    \ \   \ \`   | 1 (^ odd)
    //

    // Escaped characters are characters following an escape.
    potential_escape.store(values);
    static constexpr uint64_t shiftAmount{64 - 1};
    values[4] = values[0] << 1;
    values[1 + 4] = values[1] << 1 | values[1 - 1] >> (shiftAmount);
    values[2 + 4] = values[2] << 1 | values[2 - 1] >> (shiftAmount);
    values[3 + 4] = values[3] << 1 | values[3 - 1] >> (shiftAmount);
    simd::simd8<uint8_t> maybe_escaped =
        simd::simd8<uint8_t>::load_a(values + 4);
    const simd::simd8<uint8_t> ODD_BITS{0xAA};

    // To distinguish odd from even escape sequences, therefore, we turn on any
    // *starting* escapes that are on an odd byte. (We actually bring in all odd
    // bits, for speed.)
    // - Odd runs of backslashes are 0000, and the code at the end ("n" in \n or
    // \\n) is 1.
    // - Odd runs of backslashes are 1111, and the code at the end ("n" in \n or
    // \\n) is 0.
    // - All other odd bytes are 1, and even bytes are 0.
    simd::simd8<uint8_t> maybe_escaped_and_odd_bits = maybe_escaped | ODD_BITS;
    simd::simd8<uint8_t> even_series_codes_and_odd_bits =
        opSub(maybe_escaped_and_odd_bits, potential_escape);

    // Now we flip all odd bytes back with xor. This:
    // - Makes odd runs of backslashes go from 0000 to 1010
    // - Makes even runs of backslashes go from 1111 to 1010
    // - Sets actually-escaped codes to 1 (the n in \n and \\n: \n = 11, \\n =
    // 100)
    // - Resets all other bytes to 0
    return even_series_codes_and_odd_bits ^ ODD_BITS;
  }
};

} // namespace stage1
} // unnamed namespace
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

#endif // SIMDJSON2_SRC_GENERIC_STAGE1_JSON_STRING_SCANNER_H
