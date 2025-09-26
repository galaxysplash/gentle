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
  [[nodiscard]] consteval static inline auto get_main_cpp() noexcept
      -> std::string_view {
    return "// main.cpp\n"
           "\n"
           "#include <print>\n"
           "\n"
           "auto main(const int argc, const char *const *const argv) -> int {\n"
           "  std::println(\"hello world!\");\n"
           "}\n";
  }

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
              const std::string_view &header_name) noexcept -> std::string;

  [[nodiscard]] static auto
  get_module_cmake_lists_txt(const std::string_view &name) noexcept
      -> std::string;
};
} // namespace content