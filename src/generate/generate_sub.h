// generate_sub.h

#pragma once

#include <expected>
#include <string>

class GenerateSub final {
public:
  static auto run(const int argc, const char *const *const argv)
      -> std::expected<void, std::string>;
};