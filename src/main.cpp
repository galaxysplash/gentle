// main.cpp

#include "content.h"
#include "core_utils.h"
#include "defer.h"
#include "keyword_binding.h"

#include <utility>

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
      if (i != 0) [[unlikely]] {
        std::println("or");
      }
      std::println("expected: \"gentle {} {}\"", keyword_binding.keyword_name,
                   keyword_binding.err_msg_example_name);
    }
    std::terminate();
  }
  generate_project(argc, argv);
}
