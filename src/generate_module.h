// generate_module.h

#pragma once

#include <expected>
#include <string_view>

[[nodiscard]] auto inline generate_module(
    const int argc, const char *const *const argv) noexcept
    -> std::expected<void, std::string_view> {

  return {};
}