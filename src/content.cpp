// copyright© marcel hajek, all rights reserved.
// content.cpp

#include "content.h"

auto content::Base::get_cmake_lists_txt(const std::string_view &name) noexcept
    -> std::string {
  std::string ret;

  ret += "cmake_minimum_required(VERSION 3.30)\n"
         "project(";
  ret += name;
  ret += ")\n"
         "\n"
         "set(CMAKE_CXX_STANDARD 26)\n"
         "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n"
         "\n"
         "file(GLOB SOURCES src/*.cpp)\n";

  return ret;
}

[[nodiscard]] auto content::ProjGen::get_proj_cmake_lists_txt(
    const std::string_view &name) noexcept -> std::string {
  std::string ret;

  ret += Base::get_cmake_lists_txt(name);
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

  ret += "class ";
  ret += module_name;
  ret += " final {\n"
         "public:\n"
         "  static auto run() noexcept -> void;\n"
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
  ret += "::run() noexcept -> void {\n"
         "  // insert code here...\n"
         "}\n";
  return ret;
}

[[nodiscard]] auto content::ModuleGen::get_module_cmake_lists_txt(
    const std::string_view &name) noexcept -> std::string {
  std::string ret;

  ret += Base::get_cmake_lists_txt(name);
  ret += "add_executable(${PROJECT_NAME} ${SOURCES})\n";

  return ret;
}