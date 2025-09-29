// core_utils.cpp

#include "core_utils.h"
#include <cctype>
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

auto core_utils::CoreUtils::create_or_get_include_directory_structure(
    const std::filesystem::path &base_path,
    const std::string_view &owning_project_name,
    const std::string_view &module_name) noexcept
    -> std::expected<std::filesystem::path, std::string> {
  auto ret = std::filesystem::path{};
  if (!std::filesystem::exists(core_utils::INCLUDE_DIRECTORY_NAME))
      [[unlikely]] /* if there is no 'include' dir */ {
    {
      {
        std::println("creating include dir...");
        auto result = core_utils::CoreUtils::make_directory(
            base_path, core_utils::INCLUDE_DIRECTORY_NAME);

        if (!result) {
          return std::unexpected{result.error()};
        }
      }
      {
        std::println("creating include/{} dir...", owning_project_name);
        auto result = core_utils::CoreUtils::make_directory(
            base_path.parent_path(),
            (base_path.filename() / core_utils::INCLUDE_DIRECTORY_NAME /
             owning_project_name)
                .string());

        if (!result) {
          return std::unexpected{result.error()};
        }
      }
      {
        std::println("creating include/{}/{} dir...", owning_project_name,
                     module_name);
        auto result = core_utils::CoreUtils::make_directory(
            base_path.parent_path(),
            (base_path.filename() / core_utils::INCLUDE_DIRECTORY_NAME /
             owning_project_name / module_name)
                .string());

        if (!result) {
          return std::unexpected{result.error()};
        }

        return result;
      }
    }
  } else if (!std::filesystem::exists(base_path /
                                      core_utils::INCLUDE_DIRECTORY_NAME /
                                      owning_project_name)) [[unlikely]]
    [[unlikely]] /* the include/my_example_proj where my_example proj is
                    replaced with the folder name where currently in. and be
                    'we' I mean the user*/
    {
      {
        auto result = core_utils::CoreUtils::make_directory(
            base_path.parent_path(),
            (base_path.filename() / core_utils::INCLUDE_DIRECTORY_NAME /
             owning_project_name)
                .string());

        if (!result) {
          return std::unexpected{result.error()};
        }
      }
      {
        auto result = core_utils::CoreUtils::make_directory(
            base_path.parent_path(),
            (base_path.filename() / core_utils::INCLUDE_DIRECTORY_NAME /
             owning_project_name / module_name)
                .string());

        if (!result) {
          return std::unexpected{result.error()};
        }

        return result;
      }
    }
  else if (!std::filesystem::exists(
               base_path / core_utils::INCLUDE_DIRECTORY_NAME /
               owning_project_name / module_name)) [[likely]] {
    auto result = core_utils::CoreUtils::make_directory(
        base_path.parent_path(),
        (base_path.filename() / core_utils::INCLUDE_DIRECTORY_NAME /
         owning_project_name / module_name)
            .string());

    if (!result) {
      return std::unexpected{result.error()};
    }

    return result;
  } else [[unlikely]] {
    return base_path / core_utils::INCLUDE_DIRECTORY_NAME /
           owning_project_name / module_name;
  }
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