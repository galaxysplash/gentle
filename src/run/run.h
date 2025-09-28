// run.h

#pragma once

#include <expected>
#include <string>

class Run final {
public:
  [[nodiscard]] static auto run(const int argc,
                                const char *const *const argv) noexcept
      -> std::expected<void, std::string>;

  Run() = delete;
  Run(const Run &) = delete;
};
