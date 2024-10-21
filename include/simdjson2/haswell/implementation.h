#ifndef SIMDJSON2_HASWELL_IMPLEMENTATION_H
#define SIMDJSON2_HASWELL_IMPLEMENTATION_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#include "simdjson2/haswell/base.h"
#include "simdjson2/implementation.h"
#include "simdjson2/internal/instruction_set.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

// The constructor may be executed on any host, so we take care not to use SIMDJSON2_TARGET_HASWELL
namespace simdjson2 {
namespace haswell {

/**
 * @private
 */
class implementation final : public simdjson2::implementation {
public:
  simdjson2_inline implementation() : simdjson2::implementation(
      "haswell",
      "Intel/AMD AVX2",
      internal::instruction_set::AVX2 | internal::instruction_set::PCLMULQDQ | internal::instruction_set::BMI1 | internal::instruction_set::BMI2
  ) {}
  simdjson2_warn_unused error_code create_dom_parser_implementation(
    size_t capacity,
    size_t max_length,
    std::unique_ptr<internal::dom_parser_implementation>& dst
  ) const noexcept final;
  simdjson2_warn_unused error_code minify(const uint8_t *buf, size_t len, uint8_t *dst, size_t &dst_len) const noexcept final;
  simdjson2_warn_unused bool validate_utf8(const char *buf, size_t len) const noexcept final;
};

} // namespace haswell
} // namespace simdjson2

#endif // SIMDJSON2_HASWELL_IMPLEMENTATION_H
