// copyright© marcel hajek, all rights reserved.
// generate_class.cpp

#include "generate_class.h"
#include "content.h"
#include "core_utils.h"
#include <expected>
#include <filesystem>
#include <format>

auto GenerateClass::run(const int argc, const char *const *const argv) noexcept
    -> std::expected<void, std::string> {

  const auto module_name_result = core_utils::CoreUtils::get_name(argc, argv);

  if (!module_name_result) [[unlikely]] {
    return std::unexpected{module_name_result.error()};
  }
  const auto header_name = module_name_result.value();

  const auto src_dir =
      std::filesystem::current_path() / core_utils::SRC_DIR_NAME;

  const auto write_result = core_utils::CoreUtils::write_files({
      core_utils::File<std::string>{
          std::format("{}.cpp", header_name),
          src_dir,
          content::ClassGen::get_cpp_file(header_name),
      },
      core_utils::File<std::string>{
          std::format("{}.h", header_name),
          src_dir,
          content::ClassGen::get_h_file(
              core_utils::CoreUtils::snake_case_to_upper_case(header_name),
              header_name),
      },
  });

  if (!write_result) [[unlikely]] {
    return std::unexpected{write_result.error()};
  }

  return {};
}