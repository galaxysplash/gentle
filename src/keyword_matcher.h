// keyword_matcher.h

#pragma once

#include "keyword_binding.h"

#include <expected>
#include <initializer_list>

class KeywordMatcher final {
public:
  [[nodiscard]] static auto
  run(const int argc, const char *const *const argv,
      std::initializer_list<KeywordBinding> &&keyword_bindings)
      -> std::expected<void, std::string>;

private:
  [[nodiscard]] static auto get_modifier_argument(
      const int argc, const char *const *const argv,
      const std::initializer_list<KeywordBinding> &keyword_bindings)
      -> std::expected<std::string, std::string>;
};
