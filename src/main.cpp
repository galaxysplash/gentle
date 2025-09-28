// main.cpp

#include "generate/generate_class.h"
#include "generate/generate_module.h"
#include "generate/generate_project.h"
#include "run/build.h"
#include "run/run.h"

#include "keyword_binding.h"
#include "keyword_matcher.h"

#include <expected>
#include <format>
#include <print>
#include <string>
#include <string_view>

auto main(const int argc, const char *const *const argv) -> int {
  if (argc == 2) {
    if (std::string_view{argv[1]} == "run") [[likely]] {
      if (const auto result = Run::run(argc, argv); !result) {
        std::println("{}", result.error());
        return -1;
      } else {
        return 0;
      }
    } else if (std::string_view{argv[1]} == "build") [[unlikely]] {
      if (const auto result = Build::run(); !result) {
        std::println("{}", result.error());
        return -1;
      } else {
        return 0;
      }
    } else [[likely]] {
      std::println("either \"gentle build\" or \"gentle run\"");
      std::println(
          "or just give gentle one more argument, regardless what it \n"
          "is and it will tell you what it actually expected. or just \n"
          "no arg than it will also do that.");
    }
  }

  const auto match_keyword_result = KeywordMatcher::run(
      argc, argv,
      {
          KeywordBinding{
              "proj",
              [&argc, &argv]() noexcept -> std::expected<void, std::string> {
                return GenerateProject::run(argc, argv);
              },
          },
          KeywordBinding{
              "mod",
              [&argc, &argv]() noexcept -> std::expected<void, std::string> {
                return GenerateModule::run(argc, argv);
              },
          },
          KeywordBinding{
              "class",
              [&argc, &argv]() noexcept -> std::expected<void, std::string> {
                return GenerateClass::run(argc, argv);
              },
          },
          KeywordBinding{
              "run",
              [&argc, &argv]() noexcept -> std::expected<void, std::string> {
                return Run::run(argc, argv);
              },
          },
          KeywordBinding{
              "build",
              [&argc, &argv]() noexcept -> std::expected<void, std::string> {
                return Build::run();
              },
          },
      });

  if (!match_keyword_result) {
    std::println("{}", match_keyword_result.error());
    return -1;
  }
}