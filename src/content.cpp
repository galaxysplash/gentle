// content.cpp

#include "content.h"
#include <string_view>

auto content::Base::get_cmake_lists_txt(const std::string_view &name) noexcept
    -> std::string {
  std::string ret;

  ret += "cmake_minimum_required(VERSION 3.30)\n"
         "project(";
  ret += name;
  ret += ")\n"
         "\n"
         "set(CMAKE_CXX_STANDARD 23)\n"
         "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n"
         "add_compile_options(-Wall - Wpedantic - Wextra - Werror)\n"
         "\n";

  return ret;
}

[[nodiscard]] auto content::ProjGen::get_proj_cmake_lists_txt(
    const std::string_view &name) noexcept -> std::string {
  std::string ret;

  ret += Base::get_cmake_lists_txt(name);
  ret += "file(GLOB SOURCES src/*.cpp)\n";
  ret += "add_executable(${PROJECT_NAME} ${SOURCES})\n";

  return ret;
}

[[nodiscard]] auto
content::ModuleGen::get_mod_h(const std::string_view &module_name,
                              const std::string_view &header_name) noexcept
    -> std::string {
  std::string ret;

  ret = "// ";
  ret += header_name;
  ret += ".h\n\n";

  ret += "#pragma once\n\n";

  ret += "class ";
  ret += module_name;
  ret += "#include <expected>\n";
  ret += "#include <string>\n";
  ret += " final {\n"
         "public:\n"
         "  [[nodiscard]] static auto run() noexcept -> std::expected<void, "
         "std::string>;\n"
         "};\n";
  return ret;
}

[[nodiscard]] auto
content::ModuleGen::get_mod_cpp(const std::string_view &module_name,
                                const std::string_view &header_name) noexcept
    -> std::string {
  std::string ret;

  ret = "// ";
  ret += header_name;
  ret += ".cpp\n\n";

  ret += "#include \"";
  ret += header_name;
  ret += ".h\"\n\n";
  ret += "auto ";
  ret += module_name;
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
  ret += "target_link_libraries(${PROJECT_NAME} PUBLIC ${SOURCES})\n";

  return ret;
}

auto content::ClassGen::get_cpp_file(
    const std::string_view &class_name,
    const std::string_view &header_name) noexcept -> std::string {
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

auto content::ClassGen::get_h_file(const std::string_view &class_name,
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
  ret += "() = delete;\n";

  // delete copy ctor
  ret += "  ";
  ret += class_name;
  ret += "(const ";
  ret += class_name;
  ret += " &) = delete;\n";
  ret += "};\n";

  return ret;
}