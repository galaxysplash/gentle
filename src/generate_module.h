// copyright© marcel hajek, all rights reserved.
// generate_module.h

#pragma once

#include "core_utils.h"

#include <expected>
#include <filesystem>
#include <initializer_list>
#include <string_view>

constexpr inline auto LIB_DIRECTORY_NAME = "lib";

[[nodiscard]] inline auto
create_or_get_lib_directory(const std::filesystem::path &base_path) noexcept
    -> std::expected<std::filesystem::path, std::string> {

  if (std::filesystem::exists(std::filesystem::path{LIB_DIRECTORY_NAME}))
      [[likely]] {
    return base_path / LIB_DIRECTORY_NAME;
  }
  return core_utils::CoreUtils::make_directory(base_path, LIB_DIRECTORY_NAME);
}

[[nodiscard]] inline auto
create_mod_directory(const std::filesystem::path &base_path,
                     const std::string_view &name)
    -> std::expected<std::filesystem::path, std::string> {
  return core_utils::CoreUtils::make_directory(base_path, name);
}

[[nodiscard]] auto inline generate_module(
    const int argc, const char *const *const argv) noexcept
    -> std::expected<void, std::string> {
  const auto name_result = core_utils::CoreUtils::get_name(argc, argv);

  if (!name_result) [[unlikely]] {
    return std::unexpected{name_result.error()};
  }
  const auto name = name_result.value();
  const auto lib_directory_result = create_or_get_lib_directory(name);
  if (!lib_directory_result) [[unlikely]] {
    return std::unexpected{lib_directory_result.error()};
  }
  const auto lib_directory = lib_directory_result.value();

  const auto mod_directory_result = create_mod_directory(
      std::filesystem::current_path() / LIB_DIRECTORY_NAME, name);
  if (!mod_directory_result) {
    return std::unexpected{mod_directory_result.error()};
  }
  const auto mod_directory = mod_directory_result.value();

  const auto write_files_result = core_utils::CoreUtils::write_files(
      std::initializer_list<core_utils::File<std::string>>{});
  if (!write_files_result) {
  }
  return {};
}