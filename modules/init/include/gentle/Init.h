#pragma once

#include <expected>
#include <variant>

namespace gentle {
class Init {
 public:
  struct ModeMappingErr {
    struct NoArguments {};
    struct MalformedArguments {};
    struct TooManyArguments {};

    using VariantType =
        std::variant<NoArguments, MalformedArguments, TooManyArguments>;
    ModeMappingErr(const VariantType &&variant) noexcept : variant(variant) {}
    const VariantType variant;
  };

  enum class Mode {
    PROJ_GEN,
    MOD_GEN,
  };

  [[nodiscard("potential err may not be ignored")]] static auto
  map_to_mode() noexcept -> std::expected<Mode, ModeMappingErr>;
};
}  // namespace gentle