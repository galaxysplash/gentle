#pragma once

#include <cstdint>
#include <exception>
#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <initializer_list>
#include <print>
#include <string>
#include <string_view>

constexpr std::uint8_t MIN_ARGS_TO_GENERATE_PROJECT_NAME = 3;
constexpr std::string_view SRC_DIR_NAME = "src";

[[nodiscard]] static auto
get_project_name(const int argc, const char *const *const argv) noexcept
    -> std::expected<std::string_view, std::string_view> {
  if (argc < MIN_ARGS_TO_GENERATE_PROJECT_NAME) {
    return std::unexpected{"no arguments given."};
  }

  return argv[2];
}

[[nodiscard]] static auto make_directory(const std::filesystem::path &base_path,
                                         const std::string_view name) noexcept
    -> std::expected<std::filesystem::path, std::string_view> {
  const auto new_path = base_path / name;
  if (!std::filesystem::create_directory(new_path)) {
    return std::unexpected{std::format(
        "Failed to create project directory '{}' in path '{}'.\n\n"
        "Hint: Ensure your Operating System gives the proper permissions so "
        "it is allowed to create files and folders.",
        name, base_path.string())};
  }

  return new_path;
}

[[nodiscard]] static auto
make_project_directory(const std::string_view &project_name) noexcept
    -> std::expected<std::filesystem::path, std::string_view> {
  return make_directory(std::filesystem::current_path(), project_name);
}

[[nodiscard]] static auto
make_src_directory(const std::filesystem::path &project_path) noexcept
    -> std::expected<std::filesystem::path, std::string_view> {
  return make_directory(project_path, SRC_DIR_NAME);
}

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

template <typename ContentType>
  requires StringLike<ContentType>
[[nodiscard]] static auto
write_files(const std::initializer_list<File<ContentType>> &files) noexcept
    -> std::expected<std::ofstream, std::string_view> {
  try {
    for (const File<ContentType> &file : files) {
      std::println("creating file '{}' in '{}'...",
                   file.path.filename().string(), file.path.string());
      std::ofstream ofstream{file.path / file.name};
      ofstream << file.content;
    }
  } catch (const std::exception &e) {
    return std::unexpected{e.what()};
  }

  return {};
}
