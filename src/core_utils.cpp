// core_utils.cpp

#include "core_utils.h"
#include <cctype>
#include <expected>
#include <filesystem>
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

  const auto new_path = base_path / name;

  if (std::filesystem::exists(new_path)) {
    return std::unexpected{"path already exists."};
  }

  if (!std::filesystem::create_directory(new_path)) {
    return std::unexpected{std::format(
        "Failed to create project directory '{}' in path '{}'.\n\n"
        "Hint: Ensure your Operating System gives the proper permissions so "
        "it is allowed to create files and folders. OR You're in the right "
        "directory.",
        name, base_path.string())};
  }
  return new_path;
}

[[nodiscard]] auto core_utils::CoreUtils::snake_case_to_upper_case(
    const std::string_view &snake_case_str) noexcept -> std::string {
  std::string ret;
  ret.reserve(snake_case_str.size());

  for (bool first_time = true, previous_was_underscore = false;
       const auto &snake_case_char : snake_case_str) {
    if (previous_was_underscore || first_time) [[unlikely]] {
      ret += std::toupper(snake_case_char);
      first_time = previous_was_underscore = false;
      continue;
    }

    if (snake_case_char == '_') [[unlikely]] {
      previous_was_underscore = true;
      continue;
    }

    ret += snake_case_char;

    if (first_time) [[unlikely]] {
      first_time = false;
    }
  }

  return ret;
}