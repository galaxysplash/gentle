// generate_gpu_proj.h

#pragma once

#include <expected>
#include <string>

class GenerateGpuProj final {
public:
  [[nodiscard]] static auto run() noexcept -> std::expected<void, std::string>;
  
  GenerateGpuProj() = delete;
};
