// copyright© marcel hajek, all rights reserved.
// generate_project.h

#pragma once

#include "content.h"
#include "core_utils.h"

#include <expected>
#include <print>
#include <string_view>

[[nodiscard]] inline auto
generate_project(const int argc, const char *const *const argv) noexcept
    -> std::expected<void, std::string> {
  std::println("generate project...");
  const auto name_result = core_utils::CoreUtils::get_name(argc, argv);

  if (!name_result) [[unlikely]] {
    return std::unexpected{name_result.error()};
  }
  const auto &name = name_result.value();

  std::println("creating directories...");

  const auto directory_result =
      core_utils::CoreUtils::make_project_directory(name);

  if (!directory_result) [[unlikely]] {
    return std::unexpected{directory_result.error()};
  }
  const auto &directory = directory_result.value();

  std::println("project_directory: {}", directory.string());
  const auto src_directory_result =
      core_utils::CoreUtils::make_src_directory(directory);

  if (!src_directory_result) [[unlikely]] {
    return std::unexpected{src_directory_result.error()};
  }
  const auto &src_directory = src_directory_result.value();

  std::println("src_directory: {}", src_directory.string());

  const auto static_files = {core_utils::File<std::string_view>{
      "main.cpp", src_directory, content::proj_gen::get_main_cpp()}};

  const auto dynamic_files = {core_utils::File<std::string>{
      "CMakeLists.txt", directory,
      content::proj_gen::get_proj_cmake_lists_txt(name)}};

  std::println("\ncreating files...");

  const auto write_static_files_result =
      core_utils::CoreUtils::write_files<std::string_view>(
          std::move(static_files));

  if (!write_static_files_result) [[unlikely]] {
    return std::unexpected{write_static_files_result.error()};
  }

  const auto write_dynamic_files_result =
      core_utils::CoreUtils::write_files<std::string>(std::move(dynamic_files));

  if (!write_dynamic_files_result) [[unlikely]] {
    return std::unexpected{write_dynamic_files_result.error()};
  }

  return {};
}
