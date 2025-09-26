// copyright© marcel hajek, all rights reserved.
// generate_module.h

#pragma once

#include "core_utils.h"

#include <expected>
#include <filesystem>
#include <string_view>

constexpr inline auto LIB_DIRECTORY_NAME = "lib";

[[nodiscard]] inline auto
generate_lib_directory(const std::string_view &name) noexcept
    -> std::expected<void, std::string_view> {
  std::filesystem::create_directory(LIB_DIRECTORY_NAME);
  return {};
}

[[nodiscard]] auto inline generate_module(
    const int argc, const char *const *const argv) noexcept
    -> std::expected<void, std::string_view> {
  const auto name_result = core_utils::CoreUtils::get_name(argc, argv);

  if (!name_result) [[unlikely]] {
    return std::unexpected{name_result.error()};
  }
  const auto name = name_result.value();

  const auto lib_directory_result = generate_lib_directory(name);
  if (!lib_directory_result) [[unlikely]] {
    return std::unexpected{lib_directory_result.error()};
  }

  return {};
}