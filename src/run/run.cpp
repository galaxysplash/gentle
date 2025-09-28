// run.cpp

#include "core_utils.h"

#include <format>
#include <print>

#include "run.h"

auto Run::run(const int argc, const char *const *const argv) noexcept
    -> std::expected<void, std::string> {
  if (std::system("cmake . -B build")) {
    return std::unexpected{"cmake generation command failed."};
  }
  if (const auto make_build_cmd = "make -C build";
      std::system(make_build_cmd)) {
    std::println("make build command \"{}\" failed. falling back "
                 "to ninja",
                 make_build_cmd);

    if (const auto ninja_build_cmd = "ninja -C build";
        std::system(ninja_build_cmd)) {
      return std::unexpected{
          std::format("make's and ninja's build commands failed "
                      "failed.\n\nmake cmd: (\"{}\")\n,\n ninja "
                      "build cmd: (\"{}\")",
                      make_build_cmd, ninja_build_cmd)};
    }
  }
  const auto run_name_result = core_utils::CoreUtils::get_name(argc, argv);
  if (!run_name_result) {
    return std::unexpected{run_name_result.error()};
  }
  const auto run_name = run_name_result.value();
  const auto run_cmd = std::format("./build/{}", run_name);
  if (const auto &run_cmd_cstr = run_cmd.c_str(); std::system(run_cmd_cstr)) {
    return std::unexpected{
        std::format("run command (\"{}\") failed.", run_cmd_cstr)};
  }
  return {};
}
