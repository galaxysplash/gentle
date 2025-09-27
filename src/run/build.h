// build.h

#pragma once

#include <expected>
#include <string>

class Build final {
public:
  [[nodiscard]] static auto run() noexcept -> std::expected<void, std::string>;

  Build() = delete;
  Build(const Build &) = delete;
};
