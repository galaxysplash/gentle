// main.cpp

#include "generate_module.h"
#include "generate_project.h"
#include "keyword_binding.h"
#include "keyword_matcher.h"

#include <exception>
#include <expected>
#include <format>
#include <print>
#include <string_view>

auto main(const int argc, const char *const *const argv) noexcept -> int {
  auto keyword_bindings = {
      KeywordBinding{"proj",
                     [&argc, &argv]() -> std::expected<void, std::string_view> {
                       return generate_project(argc, argv);
                     },
                     "my_proj_name"},
      KeywordBinding{"mod",
                     [&argc, &argv]() -> std::expected<void, std::string_view> {
                       return generate_module(argc, argv);
                     },
                     "my_mod_name"}};

  const auto match_keyword_result =
      match_keyword(argc, argv, std::move(keyword_bindings));

  if (!match_keyword_result) {
    std::println("{}", match_keyword_result.error());
    std::terminate();
  }
}
