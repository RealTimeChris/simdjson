#ifndef SIMDJSON_SRC_GENERIC_STAGE1_JSON_ESCAPE_SCANNER_H

#ifndef SIMDJSON_CONDITIONAL_INCLUDE
#define SIMDJSON_SRC_GENERIC_STAGE1_JSON_ESCAPE_SCANNER_H
#include <generic/stage1/base.h>
#include <generic/stage1/buf_block_reader.h>
#endif // SIMDJSON_CONDITIONAL_INCLUDE

namespace simdjson {
namespace SIMDJSON_IMPLEMENTATION {
namespace {
namespace stage1 {

/**
 * Scans for escape characters in JSON, taking care with multiple backslashes (\\n vs. \n).
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

  /**
   * Get a mask of both escape and escaped characters (the characters following a backslash).
   *
   * @param potential_escape A mask of the character that can escape others (but could be
   *        escaped itself). e.g. block.eq('\\')
   */
  simdjson_really_inline escaped_and_escape next(simd::simd8<uint8_t> backslash) noexcept {

#if !SIMDJSON_SKIP_BACKSLASH_SHORT_CIRCUIT
    if (!backslash.test()) {
      return {next_escaped_without_backslashes(), simd::simd8<uint8_t>{}};
    }
#endif

    // |                                | Mask (shows characters instead of 1's) | Depth | Instructions        |
    // |--------------------------------|----------------------------------------|-------|---------------------|
    // | string                         | `\\n_\\\n___\\\n___\\\\___\\\\__\\\`   |       |                     |
    // |                                | `    even   odd    even   odd   odd`   |       |                     |
    // | potential_escape               | ` \  \\\    \\\    \\\\   \\\\  \\\`   | 1     | 1 (backslash & ~first_is_escaped)
    // | escape_and_terminal_code       | ` \n \ \n   \ \n   \ \    \ \   \ \`   | 5     | 5 (next_escape_and_terminal_code())
    // | escaped                        | `\    \ n    \ n    \ \    \ \   \ ` X | 6     | 7 (escape_and_terminal_code ^ (potential_escape | first_is_escaped))
    // | escape                         | `    \ \    \ \    \ \    \ \   \ \`   | 6     | 8 (escape_and_terminal_code & backslash)
    // | first_is_escaped               | `\                                 `   | 7 (*) | 9 (escape >> 63) ()
    //                                                                               (*) this is not needed until the next iteration
    simd::simd8<uint8_t> escape_and_terminal_code =
        next_escape_and_terminal_code(backslash & ~this->next_is_escaped);
    simd::simd8<uint8_t> escaped =
        escape_and_terminal_code ^ (backslash | this->next_is_escaped);
    simd::simd8<uint8_t> escape = escape_and_terminal_code & backslash;
    this->next_is_escaped = this->next_is_escaped.set_lsb(escape.get_msb());
    return {escaped, escape};
  }

private:

  simdjson_really_inline simd::simd8<uint8_t>
  next_escaped_without_backslashes() noexcept {
    simd::simd8<uint8_t> escaped = this->next_is_escaped;
    this->next_is_escaped = this->next_is_escaped.zero();
    return escaped;
  }

  /**
   * Returns a mask of the next escape characters (masking out escaped backslashes), along with
   * any non-backslash escape codes.
   *
   * \n \\n \\\n \\\\n returns:
   * \n \   \ \n \ \
   * 11 100 1011 10100
   *
   * You are expected to mask out the first bit yourself if the previous block had a trailing
   * escape.
   *
   * & the result with potential_escape to get just the escape characters.
   * ^ the result with (potential_escape | first_is_escaped) to get escaped characters.
   */
  static simdjson_really_inline simd::simd8<uint8_t>
  next_escape_and_terminal_code(
      simd::simd8<uint8_t> potential_escape) noexcept {
    // If we were to just shift and mask out any odd bits, we'd actually get a *half* right answer:
    // any even-aligned backslash runs would be correct! Odd-aligned backslash runs would be
    // inverted (\\\ would be 010 instead of 101).
    //
    // ```
    // string:              | ____\\\\_\\\\_____ |
    // maybe_escaped | ODD  |     \ \   \ \      |
    //               even-aligned ^^^  ^^^^ odd-aligned
    // ```
    //
    // Taking that into account, our basic strategy is:
    //
    // 1. Use subtraction to produce a mask with 1's for even-aligned runs and 0's for
    //    odd-aligned runs.
    // 2. XOR all odd bits, which masks out the odd bits in even-aligned runs, and brings IN the
    //    odd bits in odd-aligned runs.
    // 3. & with backslash to clean up any stray bits.
    // runs are set to 0, and then XORing with "odd":
    //
    // |                                | Mask (shows characters instead of 1's) | Instructions        |
    // |--------------------------------|----------------------------------------|---------------------|
    // | string                         | `\\n_\\\n___\\\n___\\\\___\\\\__\\\`   |
    // |                                | `    even   odd    even   odd   odd`   |
    // | maybe_escaped                  | `  n  \\n    \\n    \\\_   \\\_  \\` X | 1 (potential_escape << 1)
    // | maybe_escaped_and_odd          | ` \n_ \\n _ \\\n_ _ \\\__ _\\\_ \\\`   | 1 (maybe_escaped | odd)
    // | even_series_codes_and_odd      | `  n_\\\  _    n_ _\\\\ _     _    `   | 1 (maybe_escaped_and_odd - potential_escape)
    // | escape_and_terminal_code       | ` \n \ \n   \ \n   \ \    \ \   \ \`   | 1 (^ odd)
    //

    // Escaped characters are characters following an escape.
    alignas(32) uint64_t valuesIn[4];
    alignas(32) uint64_t valuesOut[4];
    potential_escape.store(valuesIn);
    static constexpr uint64_t shiftAmount{64 - 1};
    valuesOut[0] = valuesIn[0] << 1;
    valuesOut[1] = valuesIn[1] << 1 | valuesIn[0] >> (shiftAmount);
    valuesOut[2] = valuesIn[2] << 1 | valuesIn[1] >> (shiftAmount);
    valuesOut[3] = valuesIn[3] << 1 | valuesIn[2] >> (shiftAmount);
    simd::simd8<uint8_t> maybe_escaped = simd::simd8<uint8_t>::load(valuesOut);
    const simd::simd8<uint8_t> ODD_BITS{0xAA};

    // To distinguish odd from even escape sequences, therefore, we turn on any *starting*
    // escapes that are on an odd byte. (We actually bring in all odd bits, for speed.)
    // - Odd runs of backslashes are 0000, and the code at the end ("n" in \n or \\n) is 1.
    // - Odd runs of backslashes are 1111, and the code at the end ("n" in \n or \\n) is 0.
    // - All other odd bytes are 1, and even bytes are 0.
    simd::simd8<uint8_t> maybe_escaped_and_odd_bits = maybe_escaped | ODD_BITS;
    simd::simd8<uint8_t> even_series_codes_and_odd_bits =
        maybe_escaped_and_odd_bits - potential_escape;

    // Now we flip all odd bytes back with xor. This:
    // - Makes odd runs of backslashes go from 0000 to 1010
    // - Makes even runs of backslashes go from 1111 to 1010
    // - Sets actually-escaped codes to 1 (the n in \n and \\n: \n = 11, \\n = 100)
    // - Resets all other bytes to 0
    return even_series_codes_and_odd_bits ^ ODD_BITS;
  }
};

} // namespace stage1
} // unnamed namespace
} // namespace SIMDJSON_IMPLEMENTATION
} // namespace simdjson

#endif // SIMDJSON_SRC_GENERIC_STAGE1_JSON_STRING_SCANNER_H
