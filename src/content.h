// copyright© marcel hajek, all rights reserved.
// content.h

#pragma once

#include <string>
#include <string_view>

namespace content {
#pragma region PROGRAM_GEN
[[nodiscard]] consteval auto get_main_cpp() noexcept -> std::string_view {
  return "// main.cpp\n"
         "\n"
         "#include <print>\n"
         "\n"
         "\n"
         "auto main(const int argc, const char *const *const argv) noexcept -> "
         "int {\n"
         "  std::println(\"hello world!\");\n"
         "}\n";
}

[[nodiscard]] inline auto
get_cmake_lists_txt(const std::string_view project_name) noexcept
    -> std::string {
  std::string ret;

  ret += "cmake_minimum_required(VERSION 3.30)\n"
         "project(";
  ret += project_name;
  ret += ")\n"
         "\n"
         "set(CMAKE_CXX_STANDARD 26)\n"
         "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n"
         "\n"
         "file(GLOB SOURCES src/*.cpp)\n"
         "add_executable(${PROJECT_NAME} ${SOURCES})\n";

  return ret;
}
#pragma endregion

#pragma region MODULES_GEN
[[nodiscard]] inline auto
get_mod_h(const std::string_view &module_name,
          const std::string_view &header_name) noexcept -> std::string {
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

[[nodiscard]] inline auto
get_mod_cpp(const std::string_view &module_name,
            const std::string_view &header_name) noexcept -> std::string {
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
#pragma endregion
} // namespace content