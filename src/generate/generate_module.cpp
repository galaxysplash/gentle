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
#include <string_view>

[[nodiscard]] auto GenerateModule::run(const int argc,
                                       const char *const *const argv) noexcept
    -> std::expected<void, std::string> {

  const auto module_name_result = core_utils::CoreUtils::get_name(argc, argv);

  if (!module_name_result) [[unlikely]] {
    return std::unexpected{module_name_result.error()};
  }
  const auto module_name = module_name_result.value();

  const auto owning_project_name =
      std::filesystem::current_path().filename().string();

  if (module_name == std::filesystem::current_path().filename().string()) {
    return std::unexpected{"Err: same as ${ProjectName}.\nChose another name."};
  }

  const auto include_directory_result =
      create_or_get_include_directory_structure(
          std::filesystem::current_path(), owning_project_name, module_name);

  if (!include_directory_result) {
    return std::unexpected{include_directory_result.error()};
  }
  const auto include_directory = include_directory_result.value();

  std::println("generate module...");

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

  const auto previous_content_result =
      get_previous_cmake_lists_txt_content(std::filesystem::current_path());

  if (!previous_content_result) {
    return std::unexpected{previous_content_result.error()};
  }
  const auto previous_content = previous_content_result.value();

  const auto write_files_result = core_utils::CoreUtils::write_files(
      std::initializer_list<core_utils::File<std::string>>{
          core_utils::File<std::string>{
              std::format("{}.h", module_name),
              include_directory / module_name,
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
                      "\n# {}\n"
                      "add_subdirectory(lib/{})\n"
                      "target_link_libraries(${{PROJECT_NAME}} PRIVATE {})\n",
                      module_name, module_name, module_name, module_name),
          }});

  if (!write_files_result) {
    return std::unexpected{write_files_result.error()};
  }
  return {};
}

auto GenerateModule::create_or_get_include_directory_structure(
    const std::filesystem::path &base_path,
    const std::string_view &owning_project_name,
    const std::string_view &module_name) noexcept
    -> std::expected<std::filesystem::path, std::string> {
  auto ret = std::filesystem::path{};
  if (!std::filesystem::exists(INCLUDE_DIRECTORY_NAME))
      [[unlikely]] /* if there is no 'include' dir */ {
    {
      {
        std::println("creating include dir...");
        auto result = core_utils::CoreUtils::make_directory(
            base_path, INCLUDE_DIRECTORY_NAME);

        if (!result) {
          return std::unexpected{result.error()};
        }
      }
      {
        std::println("creating include/{} dir...", owning_project_name);
        auto result = core_utils::CoreUtils::make_directory(
            base_path, (base_path.filename() / INCLUDE_DIRECTORY_NAME /
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
            base_path, (base_path.filename() / INCLUDE_DIRECTORY_NAME /
                        owning_project_name / module_name)
                           .string());

        if (!result) {
          return std::unexpected{result.error()};
        }

        return result;
      }
    }
  } else if (!std::filesystem::exists(base_path / INCLUDE_DIRECTORY_NAME /
                                      owning_project_name)) [[unlikely]]
    [[unlikely]] /* the include/my_example_proj where my_example proj is
                    replaced with the folder name where currently in. and be
                    'we' I mean the user*/
    {
      {
        auto result = core_utils::CoreUtils::make_directory(
            base_path, (base_path.filename() / INCLUDE_DIRECTORY_NAME /
                        owning_project_name)
                           .string());

        if (!result) {
          return std::unexpected{result.error()};
        }
      }
      {
        auto result = core_utils::CoreUtils::make_directory(
            base_path, (base_path.filename() / INCLUDE_DIRECTORY_NAME /
                        owning_project_name / module_name)
                           .string());

        if (!result) {
          return std::unexpected{result.error()};
        }

        return result;
      }
    }
  else if (!std::filesystem::exists(base_path / INCLUDE_DIRECTORY_NAME /
                                    owning_project_name / module_name))
      [[likely]] {
    auto result = core_utils::CoreUtils::make_directory(
        base_path, (base_path.filename() / INCLUDE_DIRECTORY_NAME /
                    owning_project_name / module_name)
                       .string());

    if (!result) {
      return std::unexpected{result.error()};
    }

    return result;
  } else [[unlikely]] {
    return base_path / INCLUDE_DIRECTORY_NAME / owning_project_name /
           module_name;
  }
}

auto GenerateModule::get_previous_cmake_lists_txt_content(
    const std::filesystem::path &proj_path) noexcept
    -> std::expected<std::string, std::string> {
  try {
    std::ifstream ifstream;
    std::string ret;
    ret.reserve(40);
    ifstream.open(proj_path / "CMakeLists.txt");
    while (true) {
      const char c = ifstream.get();
      if (ifstream.eof()) [[unlikely]] {
        break;
      }

      ret += c;
    }
    return ret;
  } catch (const std::exception &e) {
    return std::unexpected{e.what()};
  }
}

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
