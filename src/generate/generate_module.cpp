// generate_module.cpp

#include "generate_module.h"

#include "content.h"
#include "core_utils.h"

#include <exception>
#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <print>
#include <string>

[[nodiscard]] auto GenerateModule::create_or_get_lib_directory(
    const std::filesystem::path &base_path) noexcept
    -> std::expected<std::filesystem::path, std::string> {

  if (std::filesystem::exists(base_path)) [[likely]] {
    std::println("already exists ('{}')", base_path.string());
    return base_path;
  }

  std::println("creating directory '{}'", base_path.string());
  return core_utils::CoreUtils::make_directory(base_path.parent_path(),
                                               base_path.filename().string());
}

[[nodiscard]] auto
GenerateModule::create_mod_directory(const std::filesystem::path &base_path,
                                     const std::string_view &name)
    -> std::expected<std::filesystem::path, std::string> {
  return core_utils::CoreUtils::make_directory(base_path, name);
}

[[nodiscard]] auto GenerateModule::run(const int argc,
                                       const char *const *const argv) noexcept
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
  if (!mod_directory_result) [[unlikely]] {
    return std::unexpected{mod_directory_result.error()};
  }
  const auto &mod_directory = mod_directory_result.value();

  std::println("creating mod_directory...");

  const auto upper_case_module_name =
      core_utils::CoreUtils::snake_case_to_upper_case(module_name);

  const auto owning_project_name =
      std::filesystem::current_path().filename().string();
  const auto previous_content_result =
      get_previous_content(lib_directory / "CMakeLists.txt");

  if (!previous_content_result) {
    return std::unexpected{previous_content_result.error()};
  }
  const auto previous_content = previous_content_result.value();

  const auto write_files_result = core_utils::CoreUtils::write_files(
      std::initializer_list<core_utils::File<std::string>>{
          core_utils::File<std::string>{
              std::format("{}.h", module_name),
              mod_directory,
              content::ModuleGen::get_mod_h(upper_case_module_name,
                                            module_name),
          },
          core_utils::File<std::string>{
              std::format("{}.cpp", module_name),
              mod_directory,
              content::ModuleGen::get_mod_cpp(upper_case_module_name,
                                              module_name),
          },
          core_utils::File<std::string>{
              "CMakeLists.txt",
              mod_directory,
              content::ModuleGen::get_module_cmake_lists_txt(module_name),
          },
          core_utils::File<std::string>{
              "CMakeLists.txt",
              lib_directory.parent_path(),
              previous_content +
                  std::format(
                      "\n\n# {}\n"
                      "add_subdirectory(lib/{})\n"
                      "target_link_libraries(${{PROJECT_NAME}} PRIVATE {})\n",
                      module_name, module_name, module_name),
          }});

  if (!write_files_result) {
    return std::unexpected{write_files_result.error()};
  }
  return {};
}

[[nodiscard]] auto
GenerateModule::get_previous_content(const std::filesystem::path &path) noexcept
    -> std::expected<std::string, std::string> {
  try {
    std::ifstream ifstream;
    std::string ret;
    ifstream.open(path);
    ifstream >> ret;
    return ret;
  } catch (const std::exception &e) {
    return std::unexpected{e.what()};
  }
}