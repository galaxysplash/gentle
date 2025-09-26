// copyright© marcel hajek, all rights reserved.
// generate_module.h

#pragma once

#include "content.h"
#include "core_utils.h"

#include <expected>
#include <filesystem>
#include <format>
#include <initializer_list>
#include <print>
#include <string>
#include <string_view>

constexpr inline auto HEADER_NAME = std::string_view{"mod"};
constexpr inline auto LIB_DIRECTORY_NAME = std::string_view{"lib"};

[[nodiscard]] inline auto
create_or_get_lib_directory(const std::filesystem::path &base_path) noexcept
    -> std::expected<std::filesystem::path, std::string> {

  if (std::filesystem::exists(base_path)) [[likely]] {
    std::println("already exists ('{}')", base_path.string());
    return base_path;
  }

  std::println("creating directory '{}'", base_path.string());
  return core_utils::CoreUtils::make_directory(base_path.parent_path(),
                                               base_path.filename().string());
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
  std::println("generate module...");

  const auto module_name_result = core_utils::CoreUtils::get_name(argc, argv);

  if (!module_name_result) [[unlikely]] {
    return std::unexpected{module_name_result.error()};
  }
  const auto module_name = module_name_result.value();

  const auto lib_directory_result = create_or_get_lib_directory(
      std::filesystem::current_path() / LIB_DIRECTORY_NAME);

  if (!lib_directory_result) [[unlikely]] {
    return std::unexpected{lib_directory_result.error()};
  }
  const auto lib_directory = lib_directory_result.value();
  std::println("created lib directory.");

  const auto mod_directory_result =
      create_mod_directory(lib_directory, module_name);
  if (!mod_directory_result) {
    return std::unexpected{mod_directory_result.error()};
  }
  const auto &mod_directory = mod_directory_result.value();

  std::println("creating mod_directory...");

  const auto write_files_result = core_utils::CoreUtils::write_files(
      std::initializer_list<core_utils::File<std::string>>{
          core_utils::File<std::string>{
              std::format("{}.h", module_name),
              mod_directory,
              content::get_mod_h(module_name, HEADER_NAME),
          },
          core_utils::File<std::string>{
              std::format("{}.cpp", module_name),
              mod_directory,
              content::get_mod_cpp(module_name, HEADER_NAME),
          }});

  if (!write_files_result) {
    return std::unexpected{write_files_result.error()};
  }
  return {};
}