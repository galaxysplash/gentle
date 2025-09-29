// generate_module.h

#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <string_view>

class GenerateModule final {
public:
  constexpr static inline auto LIB_DIRECTORY_NAME = std::string_view{"lib"};

  [[nodiscard]] static auto run(const int argc,
                                const char *const *const argv) noexcept
      -> std::expected<void, std::string>;

private:
  [[nodiscard]] static auto create_or_get_include_directory_structure(
      const std::filesystem::path &base_path,
      const std::string_view &owning_project_name) noexcept
      -> std::expected<std::filesystem::path, std::string>;

  [[nodiscard]] static auto
  create_or_get_lib_directory(const std::filesystem::path &base_path) noexcept
      -> std::expected<std::filesystem::path, std::string>;

  [[nodiscard]] static auto
  create_mod_directory(const std::filesystem::path &base_path,
                       const std::string_view &name)
      -> std::expected<std::filesystem::path, std::string>;

  [[nodiscard]] static auto get_previous_cmake_lists_txt_content(
      const std::filesystem::path &path) noexcept
      -> std::expected<std::string, std::string>;
};