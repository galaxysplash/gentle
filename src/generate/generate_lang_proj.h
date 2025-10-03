// generate_lang_proj.h

#pragma once

#include <expected>
#include <string>

class GenerateLangProj final {
public:
  [[nodiscard]] static auto run() noexcept -> std::expected<void, std::string>;
  
  GenerateLangProj() = delete;
};
