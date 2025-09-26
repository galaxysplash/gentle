// copyright© marcel hajek, all rights reserved.
// generate_project.h

#pragma once

#include "core_utils.h"

#include <expected>
#include <string_view>

using core_utils::CoreUtils;

class GenerateProject final {
public:
  [[nodiscard]] static auto run(const int argc,
                                const char *const *const argv) noexcept
      -> std::expected<void, std::string>;

private:
  [[nodiscard]] static auto
  make_project_directory(const std::string_view &project_name) noexcept
      -> std::expected<std::filesystem::path, std::string>;

  [[nodiscard]] static auto
  make_src_directory(const std::filesystem::path &project_path) noexcept
      -> std::expected<std::filesystem::path, std::string>;
};
