// run.cpp

#include "core_utils.h"
#include "run/build.h"

#include <expected>
#include <filesystem>
#include <format>
#include <string>

#include "run.h"

auto Run::run(const int argc, const char *const *const argv) noexcept
    -> std::expected<void, std::string> {
  if (const auto result = Build::run(); !result) {
    std::unexpected{result.error()};
  }
  auto run_name = std::string{};

  if (argc == 3) [[unlikely]] {
    const auto run_name_result = core_utils::CoreUtils::get_name(argc, argv);
    if (!run_name_result) {
      return std::unexpected{run_name_result.error()};
    }
    run_name = run_name_result.value();
  } else if (argc == 2) [[likely]] {
    run_name = std::filesystem::current_path().filename();
  } else if (argc < 2) [[unlikely]] {
    return std::unexpected{"to less args.\nwanted: \"gentle run\""};
  } else if (argc > 3) [[unlikely]] {
    return std::unexpected{"too much args.\nwanted: \"gentle run\""};
  }
  const auto run_cmd = std::format("./build/{}", run_name);
  if (const auto &run_cmd_cstr = run_cmd.c_str(); std::system(run_cmd_cstr)) {
    return std::unexpected{
        std::format("run command (\"{}\") failed.", run_cmd_cstr)};
  }
  return {};
}
