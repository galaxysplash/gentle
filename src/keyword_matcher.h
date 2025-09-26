// copyright© marcel hajek, all rights reserved.
// keyword_matcher.h

#pragma once

#include "keyword_binding.h"

#include <expected>
#include <initializer_list>

class KeywordMatcher final {
public:
  [[nodiscard]] static auto get_modifier_argument(
      const int argc, const char *const *const argv,
      const std::initializer_list<KeywordBinding> &keyword_bindings) noexcept
      -> std::expected<std::string, std::string>;

  [[nodiscard]] static auto match_keyword(
      const int argc, const char *const *const argv,
      std::initializer_list<KeywordBinding> &&keyword_bindings) noexcept
      -> std::expected<void, std::string>;
};
