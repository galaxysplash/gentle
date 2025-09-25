#pragma once

#include <functional>
#include <string_view>

struct KeywordBinding {
  explicit inline KeywordBinding(
      const std::string_view keyword_name, std::function<void()> &&lambda,
      const std::string_view err_msg_example_name) noexcept
      : keyword_name(keyword_name), lambda(lambda),
        err_msg_example_name(err_msg_example_name) {}

  const std::string_view keyword_name;
  const std::function<void()> lambda;
  const std::string_view err_msg_example_name;
};