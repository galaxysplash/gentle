// copyright© marcel hajek, all rights reserved.
// content.h

#pragma once

#include <string>
#include <string_view>

namespace content {
struct Base {
  [[nodiscard]] static auto
  get_cmake_lists_txt(const std::string_view &name) noexcept -> std::string;
};
struct ProjGen {
  [[nodiscard]] static consteval auto get_main_cpp() noexcept
      -> std::string_view;

  [[nodiscard]] static auto
  get_proj_cmake_lists_txt(const std::string_view &name) noexcept
      -> std::string;
};

struct ModuleGen {
  [[nodiscard]] static auto
  get_mod_h(const std::string_view &module_name,
            const std::string_view &header_name) noexcept -> std::string;

  [[nodiscard]] static auto
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

  [[nodiscard]] static auto
  get_module_cmake_lists_txt(const std::string_view &name) noexcept
      -> std::string {
    std::string ret;

    ret += Base::get_cmake_lists_txt(name);
    ret += "add_executable(${PROJECT_NAME} ${SOURCES})\n";

    return ret;
  }
};
} // namespace content