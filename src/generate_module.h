#pragma once

#include <expected>
#include <string_view>

auto inline generate_module() noexcept
    -> std::expected<void, std::string_view> {

  return {};
}