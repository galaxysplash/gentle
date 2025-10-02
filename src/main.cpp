// main.cpp

#include "content.h"
#include "core_utils.h"
#include "generate/generate_module.h"
#include "generate/generate_project.h"
#include "generate/generate_sub.h"
#include "run/build.h"
#include "run/run.h"

#include "keyword_binding.h"
#include "keyword_matcher.h"

#include <cstdlib>
#include <exception>
#include <expected>
#include <filesystem>
#include <format>
#include <iostream>
#include <print>
#include <string>
#include <string_view>

[[noreturn]] auto handle2args(const int argc,
                              const char *const *const argv) noexcept -> void {
  if (argc != 2) [[unlikely]] {
    std::println("called with NOT 2 arguments");
    std::terminate();
  }
  if (std::string_view{argv[1]} == "--help" ||
      std::string_view{argv[1]} == "-h" || std::string_view{argv[1]} == "help")
      [[unlikely]] {
    std::cout << R"(help:
"gentle --help" or "gentle -h" => to get to this 'help' screen

projects:
"gentle my_project" => to create a new project, where 'my_project' is the name

"gentle new my_project_name" or "gentle proj my_project_name" or "gentle project my_project_name" => to create a project, where 'my_project_name' is the name 

"gentle build" => to build your cmake project

"gentle run" => to run your cmake project

class:
"gentle class my_class_name" => to create a class, where 'my_class_name' is the name

mod:
"gentle mod my_mod_name" => to create a class, where 'my_mod_name' is the name

sub:
"gentle sub my_sub_name" => to create a class, where 'my_sub_name' is the name
)";
  } else if (std::string_view{argv[1]} == "run") [[likely]] {
    if (const auto result = Run::run(argc, argv); !result) {
      std::println("{}", result.error());
    }
  } else if (std::string_view{argv[1]} == "build") [[unlikely]] {
    if (const auto result = Build::run(); !result) {
      std::println("{}", result.error());
    }
  } else {
    constexpr int FAKE_ARGC = 3; // HAS TO BE EXACTLY 3
    const char *fake_argv[FAKE_ARGC];
    fake_argv[0] = argv[0];
    fake_argv[1] = "new";
    fake_argv[2] = argv[1];
    const auto generation_result = GenerateProject::run(FAKE_ARGC, fake_argv);

    if (!generation_result) {
      std::println("{}", generation_result.error());
    }
  }

  exit(0);
}

[[nodiscard]] auto generate_asm_project(const int argc,
                                        const char *const *const argv) noexcept
    -> std::expected<void, std::string> {
  const auto gen_result = GenerateProject::run(argc, argv);

  if (!gen_result) {
    return gen_result;
  }

  const auto project_name_result = core_utils::CoreUtils::get_name(argc, argv);

  if (!project_name_result) {
    std::println("{}", project_name_result.value());
  }
  const auto project_name = project_name_result.value();

  const auto project_path = std::filesystem::current_path() / project_name;

  const auto src_path = project_path / core_utils::SRC_DIR_NAME;
  const auto write_dynamic_files_result = core_utils::CoreUtils::write_files(
      {core_utils::File<std::string>{
           "CMakeLists.txt",
           project_path,
           std::format(R"(cmake_minimum_required(VERSION 3.30)
project({} CXX ASM)

enable_language(ASM_NASM)

set(CMAKE_BUILD_TYPE Release)
add_compile_options(-Wall -Wpedantic -Wextra -Werror)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_SOURCE_DIR src)
include_directories(include)
file(GLOB SOURCES ${{CMAKE_SOURCE_DIR}}/*.cpp)
file(GLOB ASM_FILES ${{CMAKE_SOURCE_DIR}}/*.asm)
set_source_files_properties(${{ASM_FILES}} PROPERTIES LANGUAGE ASM_NASM)

add_executable(${{PROJECT_NAME}} ${{SOURCES}} ${{ASM_FILES}}))",
                       project_name),
       },
       core_utils::File<std::string>{
           "main.cpp",
           src_path,
           content::ProjGen::get_custom_main_cpp(project_name, "main.h", R"(
auto main() -> int {
  constexpr auto N1 = 1, N2 = 1;
  std::cout << "result: " << asm_add(N1, N2) << "\n";
})"),
       },
       core_utils::File<std::string>{
           "main.h",
           project_path / core_utils::INCLUDE_DIRECTORY_NAME / project_name,
           content::ProjGen::get_custom_main_h(R"(#include <cstdint>

extern "C" auto asm_add(const std::uint64_t lhs, const std::uint64_t rhs) noexcept 
    -> std::uint8_t;
)"),
       },
       core_utils::File<std::string>{"main.asm", src_path, R"(global add

add:
  mov r8, rdi
  mov r9, rsi
  add r9, r8
  mov rax, r9
  ret)"}});

  if (!write_dynamic_files_result) {
    return std::unexpected{"writing 'CMakeLists.txt' failed."};
  }

  return {};
}

auto generate_class(const int argc, const char *const *const argv) noexcept
    -> std::expected<void, std::string> {
  const auto module_name_result = core_utils::CoreUtils::get_name(argc, argv);

  if (!module_name_result) [[unlikely]] {
    return std::unexpected{module_name_result.error()};
  }
  const auto header_name = module_name_result.value();

  const auto src_dir =
      std::filesystem::current_path() / core_utils::SRC_DIR_NAME;

  const auto class_name =
      core_utils::CoreUtils::snake_case_to_upper_case(header_name);
  const auto write_result = core_utils::CoreUtils::write_files({
      core_utils::File<std::string>{
          std::format("{}.cpp", header_name),
          src_dir,
          std::format( //
              "// {}.cpp\n\n"
              "#include \"{}.h\"\n\n"
              //
              ,
              header_name, header_name),
      },
      core_utils::File<std::string>{
          std::format("{}.h", header_name),
          src_dir,
          std::format( //
              "// {}.h\n\n"
              "#pragma once\n\n"
              "class {} {{\n"
              "public:\n"
              "  //insert here...\n"
              "}};\n"
              //
              ,
              header_name, class_name),
      },
  });

  if (!write_result) [[unlikely]] {
    return std::unexpected{write_result.error()};
  }

  return {};
}
auto main(const int argc, const char *const *const argv) -> int {
  if (argc == 2) [[unlikely]] {
    handle2args(argc, argv);
  }

  const auto match_keyword_result = KeywordMatcher::run(
      argc, argv,
      {
          KeywordBinding{
              "proj",
              [&argc, &argv]() noexcept -> std::expected<void, std::string> {
                return GenerateProject::run(argc, argv);
              },
          },
          KeywordBinding{
              "project",
              [&argc, &argv]() noexcept -> std::expected<void, std::string> {
                return GenerateProject::run(argc, argv);
              },
          },
          KeywordBinding{
              "asm_proj",
              [&argc, &argv]() noexcept -> std::expected<void, std::string> {
                return generate_asm_project(argc, argv);
              },
          },
          KeywordBinding{
              "new",
              [&argc, &argv]() noexcept -> std::expected<void, std::string> {
                return GenerateProject::run(argc, argv);
              },
          },
          KeywordBinding{
              "mod",
              [&argc, &argv]() noexcept -> std::expected<void, std::string> {
                return GenerateModule::run(argc, argv);
              },
          },
          KeywordBinding{
              "sub",
              [&argc, &argv]() noexcept -> std::expected<void, std::string> {
                return GenerateSub::run(argc, argv);
              },
          },
          KeywordBinding{
              "class",
              [&argc, &argv]() noexcept -> std::expected<void, std::string> {
                return generate_class(argc, argv);
              },
          },
          KeywordBinding{
              "run",
              [&argc, &argv]() noexcept -> std::expected<void, std::string> {
                return Run::run(argc, argv);
              },
          },
          KeywordBinding{
              "build",
              []() noexcept -> std::expected<void, std::string> {
                return Build::run();
              },
          },
      });

  if (!match_keyword_result) {
    std::println("{}", match_keyword_result.error());
    return -1;
  }
}
