// main.cpp

#include "defer.h"
#include "keyword_binding.h"
#include "project_gen.h"

#include <expected>
#include <format>
#include <print>
#include <ranges>
#include <string_view>

auto get_modifier_argument(const int argc,
                           const char *const *const argv) noexcept
    -> std::expected<std::string_view, std::string_view> {
  if (argc < 2) [[unlikely]] {
    return std::unexpected{std::format(
        "cannot get 'modifier_argument', argc is to small. ({})", argc)};
  }
  return argv[1];
}

auto main(const int argc, const char *const *const argv) noexcept -> int {
  const auto keyword_bindings = {
      KeywordBinding{"proj", [&]() {}, "my_proj_name"},
      KeywordBinding{"mod", [&]() {}, "my_mod_name"}};

  if (argc < core_utils::MIN_ARGS_TO_GENERATE_PROJECT_NAME) [[unlikely]] {
    std::println("{} args are required!\n",
                 core_utils::MIN_ARGS_TO_GENERATE_PROJECT_NAME - 1);
    for (std::uint8_t i = 0;
         const KeywordBinding &keyword_binding : keyword_bindings) {
      const auto defer_increment = Defer{[&i]() { ++i; }};
      if (i != 0) [[unlikely]] {
        std::println("or");
      }
      std::println("expected: \"gentle {} {}\"", keyword_binding.keyword_name,
                   keyword_binding.err_msg_example_name);
    }
    std::terminate();
  }

  keyword_bindings |
      std::ranges::views::filter(
          [&argv](const KeywordBinding &keyword_binding) -> bool {
            return keyword_binding.keyword_name == argv[1];
          });
  generate_project(argc, argv);
}
