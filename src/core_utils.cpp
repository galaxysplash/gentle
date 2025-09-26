// copyright© marcel hajek, all rights reserved.
// core_utils.cpp

#include "core_utils.h"
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <expected>
#include <format>
#include <string_view>

[[nodiscard]] auto
core_utils::CoreUtils::get_name(const int argc,
                                const char *const *const argv) noexcept
    -> std::expected<std::string, std::string> {
  if (argc < MIN_ARGS_TO_GENERATE_PROJECT_NAME) {
    return std::unexpected{"no arguments given."};
  }

  return argv[2];
}

[[nodiscard]] auto
core_utils::CoreUtils::make_directory(const std::filesystem::path &base_path,
                                      const std::string_view &name) noexcept
    -> std::expected<std::filesystem::path, std::string> {
  constexpr auto ERR_MSG = std::string_view{
      "Failed to create project directory '{}' in path '{}'.\n\n"
      "Hint: Ensure your Operating System gives the proper permissions so "
      "it is allowed to create files and folders. OR You're in the right "
      "directory."};
  try {
    const auto new_path = base_path / name;
    if (!std::filesystem::create_directory(new_path)) {
      return std::unexpected{std::format(ERR_MSG, name, base_path.string())};
    }
    return new_path;
  } catch (const std::exception &e) {
    return std::unexpected{
        std::format("C++ err: {}.\n\n{}", e.what(),
                    std::format(ERR_MSG, name, base_path.string()))};
  }
}

[[nodiscard]] auto core_utils::CoreUtils::make_project_directory(
    const std::string_view &project_name) noexcept
    -> std::expected<std::filesystem::path, std::string> {
  return make_directory(std::filesystem::current_path(), project_name);
}

[[nodiscard]] auto core_utils::CoreUtils::make_src_directory(
    const std::filesystem::path &project_path) noexcept
    -> std::expected<std::filesystem::path, std::string> {
  return make_directory(project_path, SRC_DIR_NAME);
}

[[nodiscard]] auto core_utils::CoreUtils::snake_case_to_upper_case(
    const std::string_view &snake_case_str) noexcept -> std::string {
  std::string ret;

  for (bool previous_was_underscore = false, first_time = true;
       const auto &snake_case_char : snake_case_str) {
    if (previous_was_underscore || first_time) [[unlikely]] {
      ret += std::toupper(snake_case_char);
      continue;
    }

    if (snake_case_char == '_') [[unlikely]] {
      previous_was_underscore = true;
      continue;
    }

    ret += snake_case_char;
    previous_was_underscore = false;

    if (first_time) [[unlikely]] {
      first_time = false;
    }
  }

  return ret;
}