// copyright© marcel hajek, all rights reserved.
// generate_module.h

#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <string_view>

namespace generate_module {
constexpr static inline auto LIB_DIRECTORY_NAME = std::string_view{"lib"};

class Funcs final {
public:
  [[nodiscard]] static auto run(const int argc,
                                const char *const *const argv) noexcept
      -> std::expected<void, std::string>;

private:
  [[nodiscard]] static auto
  create_or_get_lib_directory(const std::filesystem::path &base_path) noexcept
      -> std::expected<std::filesystem::path, std::string>;

  [[nodiscard]] static auto
  create_mod_directory(const std::filesystem::path &base_path,
                       const std::string_view &name)
      -> std::expected<std::filesystem::path, std::string>;
};
} // namespace generate_module