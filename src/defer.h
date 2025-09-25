// defer.h

#pragma once

#include <functional>

struct Defer final {
public:
  explicit constexpr inline Defer(std::function<void()> &&cleanup) noexcept
      : cleanup_(cleanup) {}

  Defer(Defer &&) = delete;
  Defer(const Defer &) = delete;

  inline ~Defer() noexcept {}

private:
  const std::function<void()> cleanup_;
};