// build.cpp

#include "build.h"

#include <filesystem>
#include <format>
#include <print>

auto Build::run() noexcept -> std::expected<void, std::string> {
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

  return {};
}
