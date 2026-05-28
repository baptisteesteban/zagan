#pragma once

#include <zagan/core/points/ndpoint.hpp>

#include <cmath>

namespace zagan
{
  using vec3 = ndpoint<3, double>;

  template <typename T>
  constexpr T dot(const ndpoint<3, T>& a, const ndpoint<3, T>& b) noexcept;

  template <typename T>
  constexpr ndpoint<3, T> cross(const ndpoint<3, T>& a, const ndpoint<3, T>& b) noexcept;

  template <typename T>
  inline T norm(const ndpoint<3, T>& a) noexcept;

  template <typename T>
  inline ndpoint<3, T> normalized(const ndpoint<3, T>& a) noexcept;

  template <typename T>
  constexpr ndpoint<3, T> operator+(const ndpoint<3, T>& a, const ndpoint<3, T>& b) noexcept;

  template <typename T>
  constexpr ndpoint<3, T> operator-(const ndpoint<3, T>& a, const ndpoint<3, T>& b) noexcept;

  template <typename T>
  constexpr ndpoint<3, T> operator*(const ndpoint<3, T>& a, T scalar) noexcept;

  template <typename T>
  constexpr ndpoint<3, T> operator*(T scalar, const ndpoint<3, T>& a) noexcept;

  /*
   * Implementation
   */

  template <typename T>
  constexpr T dot(const ndpoint<3, T>& a, const ndpoint<3, T>& b) noexcept
  {
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
  }

  template <typename T>
  constexpr ndpoint<3, T> cross(const ndpoint<3, T>& a, const ndpoint<3, T>& b) noexcept
  {
    return ndpoint<3, T>(a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x());
  }

  template <typename T>
  inline T norm(const ndpoint<3, T>& a) noexcept
  {
    return std::sqrt(dot(a, a));
  }

  template <typename T>
  inline ndpoint<3, T> normalized(const ndpoint<3, T>& a) noexcept
  {
    const T n = norm(a);
    return ndpoint<3, T>(a.x() / n, a.y() / n, a.z() / n);
  }

  template <typename T>
  constexpr ndpoint<3, T> operator+(const ndpoint<3, T>& a, const ndpoint<3, T>& b) noexcept
  {
    return ndpoint<3, T>(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
  }

  template <typename T>
  constexpr ndpoint<3, T> operator-(const ndpoint<3, T>& a, const ndpoint<3, T>& b) noexcept
  {
    return ndpoint<3, T>(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
  }

  template <typename T>
  constexpr ndpoint<3, T> operator*(const ndpoint<3, T>& a, T scalar) noexcept
  {
    return ndpoint<3, T>(a.x() * scalar, a.y() * scalar, a.z() * scalar);
  }

  template <typename T>
  constexpr ndpoint<3, T> operator*(T scalar, const ndpoint<3, T>& a) noexcept
  {
    return a * scalar;
  }
} // namespace zagan