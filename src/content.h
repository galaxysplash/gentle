// content.h

#pragma once

#include <format>
#include <string>
#include <string_view>

namespace content {
struct Base {
  [[nodiscard]] static auto
  get_cmake_lists_txt(const std::string_view &name) noexcept -> std::string;
};
struct Matcher {
  [[nodiscard]] constexpr static inline auto
  get_err_msg_example_name(const std::string_view &name) noexcept
      -> std::string {
    return std::format("my_{}_name", name);
  }
};
struct ProjGen {
  [[nodiscard]] consteval static inline auto get_main_h() noexcept
      -> std::string_view {
    return R"(// main.h

#pragma once

)";
  }

  [[nodiscard]] static auto
  get_main_cpp(const std::string_view &project_name,
               const std::string &header_name) noexcept -> std::string;

  [[nodiscard]] static auto
  get_proj_cmake_lists_txt(const std::string_view &name) noexcept
      -> std::string;
};

struct ModuleGen {
  [[nodiscard]] static auto
  get_mod_h(const std::string_view &module_name,
            const std::string_view &header_name) noexcept -> std::string;

  [[nodiscard]] static auto
  get_mod_cpp(const std::string_view &module_class_name,
              const std::string_view &header_name,
              const std::string_view &project_name) noexcept -> std::string;

  [[nodiscard]] static auto
  get_module_cmake_lists_txt(const std::string_view &name) noexcept
      -> std::string;
};
struct SubGen final {
  [[nodiscard]] static auto
  get_cpp_file(const std::string_view &class_name,
               const std::string_view &header_name) noexcept -> std::string;
  [[nodiscard]] static auto
  get_h_file(const std::string_view &class_name,
             const std::string_view &header_name) noexcept -> std::string;
};
} // namespace content