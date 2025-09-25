// main.cpp

#include "defer.h"
#include "keyword_binding.h"
#include "project_gen.h"

#include <print>

auto main(const int argc, const char *const *const argv) noexcept -> int {
  if (argc < core_utils::MIN_ARGS_TO_GENERATE_PROJECT_NAME) {
    std::println("{} args are required!\n",
                 core_utils::MIN_ARGS_TO_GENERATE_PROJECT_NAME - 1);
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
