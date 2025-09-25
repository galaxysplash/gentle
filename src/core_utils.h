#pragma once

#include <cstdint>
#include <expected>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <string>
#include <string_view>

namespace core_utils {
constexpr static std::uint8_t MIN_ARGS_TO_GENERATE_PROJECT_NAME = 3;
constexpr static std::string_view SRC_DIR_NAME = "src";

template <typename T>
concept StringLike =
    std::is_same_v<std::string, T> || std::is_same_v<std::string_view, T>;

template <typename ContentType>
  requires StringLike<ContentType>
struct File {
  explicit inline File<ContentType>(const std::string_view name,
                                    const std::filesystem::path &path,
                                    ContentType &&content) noexcept
      : name(name), path(path), content(content) {}

  File<ContentType>(const File<ContentType> &) = delete;
  explicit inline File<ContentType>(File<ContentType> &&) noexcept = default;

  std::string_view name;
  std::filesystem::path path;
  ContentType content;
};

class CoreUtils final {
public:
  [[nodiscard]] static auto
  get_project_name(const int argc, const char *const *const argv) noexcept
      -> std::expected<std::string_view, std::string_view>;

  [[nodiscard]] static auto
  make_directory(const std::filesystem::path &base_path,
                 const std::string_view name) noexcept
      -> std::expected<std::filesystem::path, std::string_view>;

  [[nodiscard]] static auto
  make_project_directory(const std::string_view &project_name) noexcept
      -> std::expected<std::filesystem::path, std::string_view>;

  [[nodiscard]] static auto
  make_src_directory(const std::filesystem::path &project_path) noexcept
      -> std::expected<std::filesystem::path, std::string_view>;

  template <typename ContentType>
    requires StringLike<ContentType>
  [[nodiscard]] static auto
  write_files(const std::initializer_list<File<ContentType>> &files) noexcept
      -> std::expected<std::ofstream, std::string_view>;
};
} // namespace core_utils
