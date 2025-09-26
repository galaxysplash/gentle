// copyright© marcel hajek, all rights reserved.
// main.cpp

#include "generate/generate_module.h"
#include "generate/generate_project.h"
#include "keyword_binding.h"
#include "keyword_matcher.h"

#include <expected>
#include <format>
#include <print>
#include <string>
#include <string_view>

using generate_module::GenerateModule;

auto main(const int argc, const char *const *const argv) noexcept -> int {
  const auto match_keyword_result = KeywordMatcher::run(
      argc, argv,
      {KeywordBinding{
           "proj",
           [&argc, &argv]() noexcept -> std::expected<void, std::string> {
             return GenerateProject::run(argc, argv);
           },
           "my_proj_name"},
       KeywordBinding{
           "mod",
           [&argc, &argv]() noexcept -> std::expected<void, std::string> {
             return GenerateModule::run(argc, argv);
           },
           "my_mod_name"}});

  if (!match_keyword_result) {
    std::println("{}", match_keyword_result.error());
    return -1;
  }
}