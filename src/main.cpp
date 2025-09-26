// copyright© marcel hajek, all rights reserved.
// main.cpp

#include "generate_module.h"
#include "generate_project.h"
#include "keyword_binding.h"
#include "keyword_matcher.h"

#include <expected>
#include <format>
#include <print>
#include <string_view>

auto main(const int argc, const char *const *const argv) noexcept -> int {
  const auto match_keyword_result = match_keyword(
      argc, argv,
      {KeywordBinding{"proj",
                      [&argc, &argv]() -> std::expected<void, std::string> {
                        return generate_project(argc, argv);
                      },
                      "my_proj_name"},
       KeywordBinding{"mod",
                      [&argc, &argv]() -> std::expected<void, std::string> {
                        return generate_module(argc, argv);
                      },
                      "my_mod_name"}});

  if (!match_keyword_result) {
    std::println("{}", match_keyword_result.error());
    return -1;
  }
}
