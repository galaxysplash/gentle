// keyword_matcher.cpp

#include "keyword_matcher.h"
#include "content.h"
#include "core_utils.h"

#include <cstddef>
#include <format>
#include <print>
#include <ranges>
#include <string_view>

[[nodiscard]] auto KeywordMatcher::get_modifier_argument(
    const int argc, const char *const *const argv,
    const std::initializer_list<KeywordBinding> &keyword_bindings) noexcept
    -> std::expected<std::string, std::string> {
  const auto f = [&]() {
    auto unexpected_ret = std::string{};
    std::println("expected: ");
    for (std::uint8_t i = 0;
         const KeywordBinding &keyword_binding : keyword_bindings) {
      if (i != 0) [[likely]] {
        unexpected_ret += "\n or \n";
      }
      unexpected_ret +=
          std::format("\"gentle {} {}\"", keyword_binding.keyword_name,
                      content::Matcher::get_err_msg_example_name(
                          keyword_binding.keyword_name));
      ++i;
    }
    return std::unexpected{unexpected_ret};
  };

  if (argc < core_utils::MIN_ARGS_TO_GENERATE_PROJECT_NAME) [[unlikely]] {
    return f();
  }

  bool one_was_fitting = false;
  for (std::size_t i = 0; i < argc; ++i) {
    for (const auto &binding : keyword_bindings) {
      if (std::string_view{argv[i]} == binding.keyword_name) {
        one_was_fitting = true;
        break;
      }
    }
    if (one_was_fitting) {
      break;
    }
  }
  if (!one_was_fitting) {
    return f();
  }

  return argv[1];
}

[[nodiscard]] auto KeywordMatcher::run(
    const int argc, const char *const *const argv,
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