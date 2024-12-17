#ifndef EXOSOURCING_HPP
#define EXOSOURCING_HPP

#include <exosourcing/exosourcing_export.hpp>

[[nodiscard]] EXOSOURCING_EXPORT int factorial(int) noexcept;

[[nodiscard]] constexpr int factorial_constexpr(int input) noexcept
{
  if (input == 0) { return 1; }

  return input * factorial_constexpr(input - 1);
}

#endif
