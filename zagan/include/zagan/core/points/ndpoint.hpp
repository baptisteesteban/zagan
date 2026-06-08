#pragma once

#include <zagan/core/vec.hpp>

#include <cassert>
#include <cstring>

namespace zagan
{
  template <std::size_t N, typename T>
  class ndpoint : public vec<T, N>
  {
  public:
    using vec<T, N>::vec;

    constexpr ndpoint(T x) noexcept
      requires(N == 1);
    constexpr ndpoint(T x, T y) noexcept
      requires(N == 2);
    using vec<T, N>::operator=;

    using vec<T, N>::operator[];
    constexpr const T& x() const noexcept;
    constexpr const T& y() const noexcept
      requires(N >= 2);

  public:
    static constexpr std::size_t ndim() noexcept { return N; };
  };

  /*
   * Implementation
   */

  template <std::size_t N, typename T>
  constexpr ndpoint<N, T>::ndpoint(T x) noexcept
    requires(N == 1)
  {
    this->m_values[0] = x;
  }

  template <std::size_t N, typename T>
  constexpr ndpoint<N, T>::ndpoint(T x, T y) noexcept
    requires(N == 2)
  {
    this->m_values[0] = x;
    this->m_values[1] = y;
  }

  template <std::size_t N, typename T>
  constexpr const T& ndpoint<N, T>::x() const noexcept
  {
    return this->operator[](0);
  }

  template <std::size_t N, typename T>
  constexpr const T& ndpoint<N, T>::y() const noexcept
    requires(N >= 2)
  {
    return this->operator[](1);
  }
} // namespace zagan