// copyright© galaxysplash
// keyword_binding.h

#pragma once

#include <expected>
#include <functional>
#include <string_view>

struct KeywordBinding {
  explicit inline KeywordBinding(
      const std::string_view keyword_name,
      std::function<std::expected<void, std::string_view>()> &&lambda,
      const std::string_view err_msg_example_name) noexcept
      : keyword_name(keyword_name), lambda(lambda),
        err_msg_example_name(err_msg_example_name) {}

  const std::string_view keyword_name;
  const std::function<std::expected<void, std::string_view>()> lambda;
  const std::string_view err_msg_example_name;
};