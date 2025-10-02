// generate_project.cpp

#include "generate_project.h"
#include "content.h"
#include "core_utils.h"

#include <expected>
#include <filesystem>
#include <print>
#include <string_view>

[[nodiscard]] auto GenerateProject::run(const int argc,
                                        const char *const *const argv) noexcept
    -> std::expected<void, std::string> {
  std::println("generate project...");
  const auto name_result = CoreUtils::get_name(argc, argv);

  if (!name_result) [[unlikely]] {
    return std::unexpected{name_result.error()};
  }
  const auto &name = name_result.value();

  if (std::filesystem::exists(std::filesystem::current_path() / name))
      [[unlikely]] {
    return std::unexpected{"a directory with this name already exits.\nso I "
                           "will not create a new one. skipping...\n\nHINT: "
                           "PLEASE chose another project_name, ok!?!?!"};
  }

  std::println("creating directories...");

  const auto directory_result = make_project_directory(name);

  if (!directory_result) [[unlikely]] {
    return std::unexpected{directory_result.error()};
  }
  const auto &directory = directory_result.value();

  std::filesystem::create_directory(directory /
                                    core_utils::INCLUDE_DIRECTORY_NAME);
  std::filesystem::create_directory(directory /
                                    core_utils::INCLUDE_DIRECTORY_NAME / name);

  std::println("project_directory: {}", directory.string());
  const auto src_directory_result = make_src_directory(directory);

  if (!src_directory_result) [[unlikely]] {
    return std::unexpected{src_directory_result.error()};
  }
  const auto &src_directory = src_directory_result.value();

  std::println("src_directory: {}", src_directory.string());

  const auto include_dir =
      directory / core_utils::INCLUDE_DIRECTORY_NAME / name;
  const auto main_h_name = std::string_view{"main.h"};
  const auto static_files = {
      core_utils::File<std::string_view>{
          main_h_name,
          include_dir,
          content::ProjGen::get_main_h(),
      },
      core_utils::File<std::string_view>{
          ".gitignore",
          directory,
          "build",
      },
      core_utils::File<std::string_view>{
          ".clang-format",
          directory,
          "---\n"
          "BasedOnStyle: LLVM\n",
      },
  };

  const auto dynamic_files = {
      core_utils::File<std::string>{
          "main.cpp",
          src_directory,
          content::ProjGen::get_main_cpp(name, main_h_name.data()),
      },
      core_utils::File<std::string>{
          "CMakeLists.txt",
          directory,
          content::ProjGen::get_proj_cmake_lists_txt(name),
      },
  };

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

[[nodiscard]] auto GenerateProject::make_project_directory(
    const std::string_view &project_name) noexcept
    -> std::expected<std::filesystem::path, std::string> {
  return CoreUtils::make_directory(std::filesystem::current_path(),
                                   project_name);
}

[[nodiscard]] auto GenerateProject::make_src_directory(
    const std::filesystem::path &project_path) noexcept
    -> std::expected<std::filesystem::path, std::string> {
  return CoreUtils::make_directory(project_path, core_utils::SRC_DIR_NAME);
}