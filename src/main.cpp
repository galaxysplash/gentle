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

constexpr std::uint8_t MIN_ARGS_TO_GENERATE_PROJECT_NAME = 2;

[[nodiscard]] auto get_project_name(const int argc,
                                    const char *const *const argv) noexcept
    -> std::expected<std::string_view, std::string_view> {
  if (argc < MIN_ARGS_TO_GENERATE_PROJECT_NAME) {
    return std::unexpected{"no arguments given."};
  }

  return argv[1];
}

[[nodiscard]] auto make_directory(const std::string_view name) noexcept
    -> std::expected<std::filesystem::path, std::string_view> {
  name;
}

[[nodiscard]] auto
make_project_directory(const std::string_view &project_name) noexcept
    -> std::expected<std::filesystem::path, std::string_view> {

  std::filesystem::path current_path = std::filesystem::current_path();
  const bool creation_success =
      std::filesystem::create_directories(project_name);

  if (!creation_success) {
    std::unexpected{
        "Failed to create project directory.\n\n"
        "Hint: Ensure your Operating System gives the proper permissions so "
        "it is allowed to create files and folders."};
  }

  return current_path / project_name;
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
write_files(std::initializer_list<File<ContentType> &&> &&files) noexcept
    -> std::expected<std::ofstream, std::string_view> {
  try {
    for (const File<ContentType> &file : files) {
      std::ofstream ofstream{file.path};
      ofstream << file.content;
    }
  } catch (const std::exception &e) {
    return std::unexpected{e.what()};
  }
}

auto main(const int argc, const char *const *const argv) noexcept -> int {
  const auto project_name_result = get_project_name(argc, argv);

  if (!project_name_result) {
    std::println("{}", project_name_result.error());
    std::terminate();
  }

  const auto project_path_result =
      make_project_directory(project_name_result.value());

  if (!project_path_result) {
    std::println("{}", project_path_result.error());
    std::terminate();
  }

  const File<std::string_view> main_cpp{get_main_cpp()};
  const std::string cmake_lists_txt =
      get_cmake_lists_txt(project_name_result.value());
}
