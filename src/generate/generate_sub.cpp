// generate_sub.cpp

#include "generate_sub.h"
#include "content.h"
#include "core_utils.h"
#include <expected>
#include <filesystem>
#include <format>

auto GenerateSub::run(const int argc, const char *const *const argv)
    -> std::expected<void, std::string> {

  const auto module_name_result = core_utils::CoreUtils::get_name(argc, argv);

  if (!module_name_result) [[unlikely]] {
    return std::unexpected{module_name_result.error()};
  }
  const auto header_name = module_name_result.value();

  const auto src_dir =
      std::filesystem::current_path() / core_utils::SRC_DIR_NAME;

  const auto class_name =
      core_utils::CoreUtils::snake_case_to_upper_case(header_name);
  const auto write_result = core_utils::CoreUtils::write_files({
      core_utils::File<std::string>{
          std::format("{}.cpp", header_name),
          src_dir,
          content::SubGen::get_cpp_file(class_name, header_name),
      },
      core_utils::File<std::string>{
          std::format("{}.h", header_name),
          src_dir,
          content::SubGen::get_h_file(class_name, header_name),
      },
  });

  if (!write_result) [[unlikely]] {
    return std::unexpected{write_result.error()};
  }

  return {};
}