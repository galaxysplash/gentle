// copyright© marcel hajek, all rights reserved.
// generate_class.h

#pragma once

#include <expected>
#include <string>

class GenerateClass final {
public:
  static auto run(const int argc, const char *const *const argv) noexcept
      -> std::expected<void, std::string>;
};