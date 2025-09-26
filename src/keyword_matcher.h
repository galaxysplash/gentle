// copyright© marcel hajek, all rights reserved.
// keyword_matcher.h

#pragma once

#include "core_utils.h"
#include "defer.h"
#include "keyword_binding.h"

#include <expected>
#include <format>
#include <initializer_list>
#include <print>
#include <ranges>
#include <string_view>

[[nodiscard]] inline auto get_modifier_argument(
    const int argc, const char *const *const argv,
    const std::initializer_list<KeywordBinding> &keyword_bindings) noexcept
    -> std::expected<std::string, std::string> {
  if (argc < core_utils::MIN_ARGS_TO_GENERATE_PROJECT_NAME) [[unlikely]] {
    std::println("{} args are required!\n",
                 core_utils::MIN_ARGS_TO_GENERATE_PROJECT_NAME - 1);
    auto unexpected_ret = std::string{};
    for (std::uint8_t i = 0;
         const KeywordBinding &keyword_binding : keyword_bindings) {
      const auto defer_increment = Defer{[&i]() { ++i; }};
      if (i != 0) [[unlikely]] {
        std::println("or");
      }
      unexpected_ret = std::format("expected: \"gentle {} {}\"",
                                   keyword_binding.keyword_name,
                                   keyword_binding.err_msg_example_name);
    }
    return std::unexpected{unexpected_ret};
  }

  return argv[1];
}

[[nodiscard]] inline auto
match_keyword(const int argc, const char *const *const argv,
              std::initializer_list<KeywordBinding> &&keyword_bindings) noexcept
    -> std::expected<void, std::string> {
  // security check
  const auto get_modifier_argument_result =
      get_modifier_argument(argc, argv, keyword_bindings);
  if (!get_modifier_argument_result) [[unlikely]] {
    return std::unexpected{get_modifier_argument_result.error()};
  }
  const auto modifier_argument = get_modifier_argument_result.value();

  // match
  for (const KeywordBinding &matched_bindings :
       keyword_bindings |
           std::ranges::views::filter(
               [&modifier_argument](
                   const KeywordBinding &keyword_binding) -> bool {
                 return keyword_binding.keyword_name == modifier_argument;
               })) {
    return matched_bindings.lambda();
  }

  return {};
}
