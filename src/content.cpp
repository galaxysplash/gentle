// content.cpp

#include "content.h"

#include <string_view>

auto content::Base::get_cmake_lists_txt(const std::string_view &name) noexcept
    -> std::string {
  std::string ret;

  ret += "cmake_minimum_required(VERSION 3.30)\n"
         "project(";
  ret += name;
  ret += ")\n\n"
         "if(MSVC)\n"
         "  message(FATAL_ERROR \"DUMB MSVC is EXCLUDED. USE Clang or GCC. "
         "MSVC SUCKS "
         "ASS!\")\n"
         "endif()\n"
         "set(CMAKE_BUILD_TYPE "
         "Release)\n"
         "add_compile_options(-"
         "fno-exceptions -Wall "
         "-Wpedantic -Wextra "
         "-Werror)\n"
         "set(CMAKE_CXX_STANDARD "
         "23)\n"
         "set(CMAKE_CXX_STANDARD_"
         "REQUIRED ON)\n"
         "\n";

  return ret;
}

[[nodiscard]] auto content::ProjGen::get_proj_cmake_lists_txt(
    const std::string_view &name) noexcept -> std::string {
  std::string ret;

  ret += Base::get_cmake_lists_txt(name);
  ret += "include_directories(include)\n";
  ret += "file(GLOB SOURCES src/*.cpp)\n";
  ret += "add_executable(${PROJECT_NAME} ${SOURCES})\n";

  return ret;
}

auto content::ProjGen::get_entry_main_cpp(
    const std::string_view &project_name,
    const std::string_view &header_name) noexcept -> std::string {
  std::string ret;
  ret += get_custom_main_cpp(
      project_name, header_name,
      R"(auto main(const int argc, const char *const *const argv) -> int {
  std::cout << "arguments:\n";

  for (int i = 0; i < argc; ++i) {
    std::cout << argv[i] << "\n";
  }
})");

  return ret;
}

auto content::ProjGen::get_custom_main_cpp(
    const std::string_view &project_name, const std::string_view &header_name,
    const std::string_view &custom_content) noexcept -> std::string {
  std::string ret;

  ret += R"(// main.cpp
  
#include ")";
  // | up and down: #include "project_name/main.h"
  ret += project_name;
  ret += "/";
  ret += header_name;
  ret += R"("

#include <iostream>
  
)";

  ret += custom_content;

  return ret;
}

auto content::ProjGen::get_custom_main_h(
    const std::string_view &content) noexcept -> std::string {
  std::string ret;

  ret += get_main_h_base();

  ret += content;

  return ret;
}

auto content::ModuleGen::get_mod_h(const std::string_view &module_name,
                                   const std::string_view &header_name) noexcept
    -> std::string {
  std::string ret;

  ret = "// ";
  ret += header_name;
  ret += ".h\n\n";

  ret += "#pragma once\n\n";

  ret += "#include <expected>\n";
  ret += "#include <string>\n\n";

  ret += "class ";
  ret += module_name;
  ret += " final {\n"
         "public:\n"
         "  [[nodiscard]] static auto run() noexcept -> std::expected<void, "
         "std::string>;\n"
         "  ";
  ret += module_name;
  ret += "() = delete;"
         "\n"
         "};\n";
  return ret;
}

[[nodiscard]] auto
content::ModuleGen::get_mod_cpp(const std::string_view &module_class_name,
                                const std::string_view &header_name,
                                const std::string_view &project_name) noexcept
    -> std::string {
  std::string ret;

  ret = "// ";
  ret += header_name;
  ret += ".cpp\n\n";

  ret += "#include \"";
  ret += project_name;
  ret += "/";
  ret += header_name;
  ret += "/";
  ret += header_name;
  ret += ".h\"\n\n";
  ret += "auto ";
  ret += module_class_name;
  ret += "::run() noexcept -> std::expected<void, std::string> {\n"
         "  // insert code here...\n"
         "  return {};\n"
         "}\n";
  return ret;
}

[[nodiscard]] auto content::ModuleGen::get_module_cmake_lists_txt(
    const std::string_view &name) noexcept -> std::string {
  std::string ret;

  ret += Base::get_cmake_lists_txt(name);
  ret += "file(GLOB SOURCES *.cpp)\n";
  ret += "add_library(${PROJECT_NAME} ${SOURCES})\n";

  return ret;
}

auto content::SubGen::get_h_file(const std::string_view &class_name,
                                 const std::string_view &header_name) noexcept
    -> std::string {
  std::string ret;

  ret += "// ";
  ret += header_name;
  ret += ".h\n\n";

  ret += "#pragma once\n\n";

  ret += "#include <expected>\n";
  ret += "#include <string>\n\n";

  ret += "class ";
  ret += class_name;
  ret += " final {\n"
         "public:\n"
         "  [[nodiscard]] static auto run() noexcept -> std::expected<void, "
         "std::string>;\n"
         "  \n";

  // delete ctor
  ret += "  ";
  ret += class_name;
  ret += "() = delete;\n};\n";

  return ret;
}

auto content::SubGen::get_cpp_file(const std::string_view &class_name,
                                   const std::string_view &header_name) noexcept
    -> std::string {
  std::string ret;

  ret += "// ";
  ret += header_name;
  ret += ".cpp\n\n";
  ret += "#include \"";
  ret += header_name;
  ret += ".h\"\n\n";
  ret += "auto ";
  ret += class_name;
  ret += "::run() noexcept -> std::expected<void, std::string> {\n"
         "  // insert code here...\n"
         "  return {};\n"
         "}\n";

  return ret;
}