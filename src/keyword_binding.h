// keyword_binding.h

#pragma once

#include <expected>
#include <functional>
#include <string_view>

struct KeywordBinding {
  explicit inline KeywordBinding(
      const std::string_view keyword_name,
      std::function<std::expected<void, std::string>()> &&lambda) noexcept
      : keyword_name(keyword_name), lambda(lambda) {}

  const std::string_view keyword_name;
  const std::function<std::expected<void, std::string>()> lambda;
};