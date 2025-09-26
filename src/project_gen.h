#pragma once

#include "content.h"
#include "core_utils.h"

#include <print>

inline auto generate_project(const int argc,
                             const char *const *const argv) noexcept -> void {
  const auto project_name_result =
      core_utils::CoreUtils::get_project_name(argc, argv);

  if (!project_name_result) [[unlikely]] {
    std::println("{}", project_name_result.error());
    std::terminate();
  }
  const auto project_name = project_name_result.value();

  std::println("creating directories...");

  const auto project_path_result =
      core_utils::CoreUtils::make_project_directory(project_name);

  if (!project_path_result) [[unlikely]] {
    std::println("{}", project_path_result.error());
    std::terminate();
  }
  const auto project_directory = project_path_result.value();

  std::println("project_directory: {}", project_directory.string());
  const auto src_directory_result =
      core_utils::CoreUtils::make_src_directory(project_directory);

  if (!src_directory_result) [[unlikely]] {
    std::println("{}", project_path_result.error());
    std::terminate();
  }
  const auto src_directory = src_directory_result.value();

  std::println("src_directory: {}", src_directory.string());

  const auto static_files = {core_utils::File<std::string_view>{
      "main.cpp", src_directory, content::get_main_cpp()}};

  const auto dynamic_files = {core_utils::File<std::string>{
      "CMakeLists.txt", project_directory,
      content::get_cmake_lists_txt(project_name)}};

  std::println("\ncreating files...");

  const auto write_static_files_result =
      core_utils::CoreUtils::write_files<std::string_view>(
          std::move(static_files));

  if (!write_static_files_result) [[unlikely]] {
    std::println("{}", write_static_files_result.error());
    std::terminate();
  }

  const auto write_dynamic_files_result =
      core_utils::CoreUtils::write_files<std::string>(std::move(dynamic_files));

  if (!write_dynamic_files_result) [[unlikely]] {
    std::println("{}", write_dynamic_files_result.error());
    std::terminate();
  }
}
