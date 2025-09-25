// main.cpp

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
#include <utility>

constexpr std::uint8_t MIN_ARGS_TO_GENERATE_PROJECT_NAME = 2;
constexpr std::string_view SRC_DIR_NAME = "src";

[[nodiscard]] auto get_project_name(const int argc,
                                    const char *const *const argv) noexcept
    -> std::expected<std::string_view, std::string_view> {
  if (argc < MIN_ARGS_TO_GENERATE_PROJECT_NAME) {
    return std::unexpected{"no arguments given."};
  }

  return argv[1];
}

[[nodiscard]] auto make_directory(const std::filesystem::path &base_path,
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

[[nodiscard]] auto
make_project_directory(const std::string_view &project_name) noexcept
    -> std::expected<std::filesystem::path, std::string_view> {
  return make_directory(std::filesystem::current_path(), project_name);
}

[[nodiscard]] auto
make_src_directory(const std::filesystem::path &project_path) noexcept
    -> std::expected<std::filesystem::path, std::string_view> {
  return make_directory(project_path, SRC_DIR_NAME);
}

[[nodiscard]] consteval auto get_main_cpp() noexcept -> std::string_view {
  return R"(
// main.cpp

#include <print>

auto main(const int argc, const char *const *const argv) noexcept -> int {
  std::println("hello world!");
}
)";
}

[[nodiscard]] auto
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

template <typename T>
concept StringLike =
    std::is_same_v<std::string, T> || std::is_same_v<std::string_view, T>;

template <typename ContentType>
  requires StringLike<ContentType>
struct File {
  explicit inline File<ContentType>(const std::filesystem::path &path,
                                    ContentType &&content) noexcept
      : path(path), content(content) {}

  File<ContentType>(const File<ContentType> &) = delete;
  explicit inline File<ContentType>(File<ContentType> &&) noexcept = default;

  std::filesystem::path path;
  ContentType content;
};

template <typename ContentType>
  requires StringLike<ContentType>
[[nodiscard]] auto
write_files(const std::initializer_list<File<ContentType>> &files) noexcept
    -> std::expected<std::ofstream, std::string_view> {
  try {
    for (const File<ContentType> &file : files) {
      std::println("creating file '{}' in '{}'...",
                   file.path.filename().string(), file.path.string());
      std::ofstream ofstream{file.path};
      ofstream << file.content;
    }
  } catch (const std::exception &e) {
    return std::unexpected{e.what()};
  }

  return {};
}

auto main(const int argc, const char *const *const argv) noexcept -> int {
  const auto project_name_result = get_project_name(argc, argv);

  if (!project_name_result) {
    std::println("{}", project_name_result.error());
    std::terminate();
  }
  const auto project_name = project_name_result.value();

  std::println("creating directories...");

  const auto project_path_result = make_project_directory(project_name);

  if (!project_path_result) {
    std::println("{}", project_path_result.error());
    std::terminate();
  }
  const auto project_directory = project_path_result.value();

  std::println("project_directory: {}", project_directory.string());
  const auto src_directory_result = make_src_directory(project_directory);

  if (!src_directory_result) {
    std::println("{}", project_path_result.error());
    std::terminate();
  }
  const auto src_directory = src_directory_result.value();

  std::println("src_directory: {}", src_directory.string());

  const auto static_files = {
      File<std::string_view>{src_directory, get_main_cpp()}};

  const auto dynamic_files = {
      File<std::string>{project_directory, get_cmake_lists_txt(project_name)}};

  std::println("\ncreating files...");

  const auto write_static_files_result =
      write_files<std::string_view>(std::move(static_files));

  if (!write_static_files_result) {
    std::println("{}", write_static_files_result.error());
    std::terminate();
  }

  const auto write_dynamic_files_result =
      write_files<std::string>(std::move(dynamic_files));

  if (!write_dynamic_files_result) {
    std::println("{}", write_dynamic_files_result.error());
    std::terminate();
  }
}
