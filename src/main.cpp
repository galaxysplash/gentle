// main.cpp

#include "content.h"
#include "defer.h"

#include <cstdint>
#include <exception>
#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <print>
#include <string>
#include <string_view>
#include <utility>

struct KeywordBinding {
  explicit inline KeywordBinding(
      const std::string_view keyword_name, std::function<void()> &&lambda,
      const std::string_view err_msg_example_name) noexcept
      : keyword_name(keyword_name), lambda(lambda),
        err_msg_example_name(err_msg_example_name) {}

  const std::string_view keyword_name;
  const std::function<void()> lambda;
  const std::string_view err_msg_example_name;
};

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

static auto generate_project(const int argc,
                             const char *const *const argv) noexcept -> void {
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

  const auto static_files = {File<std::string_view>{"main.cpp", src_directory,
                                                    content::get_main_cpp()}};

  const auto dynamic_files = {
      File<std::string>{"CMakeLists.txt", project_directory,
                        content::get_cmake_lists_txt(project_name)}};

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

auto main(const int argc, const char *const *const argv) noexcept -> int {
  if (argc < MIN_ARGS_TO_GENERATE_PROJECT_NAME) {
    std::println("{} args are required!\n",
                 MIN_ARGS_TO_GENERATE_PROJECT_NAME - 1);
    for (std::uint8_t i = 0;
         const KeywordBinding &keyword_binding :
         std::initializer_list{KeywordBinding{"proj", [&]() {}, "my_proj_name"},
                               KeywordBinding{"mod", [&]() {},

                                              "my_mod_name"}}) {
      const auto defer_increment = Defer{[&i]() { ++i; }};
      if (i != 0) [[likely]] {
        std::println("or");
      }
      std::println("expected: \"gentle {} {}\"", keyword_binding.keyword_name,
                   keyword_binding.err_msg_example_name);
    }
    std::terminate();
  }
  generate_project(argc, argv);
}
