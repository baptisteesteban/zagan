#pragma once

#include <algorithm>
#include <cassert>
#include <cstring>
#include <initializer_list>

namespace zagan
{
  template <typename T, std::size_t N>
  class vec
  {
  public:
    using value_t                    = T;
    static constexpr std::size_t dim = N;

  public:
    constexpr vec() noexcept;
    constexpr vec(std::initializer_list<T>) noexcept;

    constexpr vec(const vec& other) noexcept;
    constexpr vec(vec&& other) noexcept;
    constexpr vec& operator=(const vec& other) noexcept;
    constexpr vec& operator=(vec&& other) noexcept;

    constexpr T&       operator[](std::size_t i) noexcept;
    constexpr const T& operator[](std::size_t i) const noexcept;

    constexpr bool operator==(const vec& other) const noexcept;
    constexpr bool operator!=(const vec& other) const noexcept;

  protected:
    T m_values[N];
  };

  /*
   * Implementation
   */

  template <typename T, std::size_t N>
  constexpr vec<T, N>::vec() noexcept
  {
    std::memset(m_values, 0, N * sizeof(T));
  }

  template <typename T, std::size_t N>
  constexpr vec<T, N>::vec(std::initializer_list<T> ini) noexcept
  {
    assert(ini.size() == N);
    std::ranges::copy(ini, m_values);
  }

  template <typename T, std::size_t N>
  constexpr vec<T, N>::vec(const vec& other) noexcept
  {
    std::memcpy(m_values, other.m_values, N * sizeof(T));
  }

  template <typename T, std::size_t N>
  constexpr vec<T, N>::vec(vec&& other) noexcept
  {
    std::ranges::move(other.m_values, m_values);
  }

  template <typename T, std::size_t N>
  constexpr vec<T, N>& vec<T, N>::operator=(const vec& other) noexcept
  {
    std::memcpy(m_values, other.m_values, N * sizeof(T));
    return *this;
  }

  template <typename T, std::size_t N>
  constexpr vec<T, N>& vec<T, N>::operator=(vec&& other) noexcept
  {
    std::ranges::move(other.m_values, m_values);
    return *this;
  }

  template <typename T, std::size_t N>
  constexpr T& vec<T, N>::operator[](std::size_t i) noexcept
  {
    assert(i < N);
    return m_values[i];
  }

  template <typename T, std::size_t N>
  constexpr const T& vec<T, N>::operator[](std::size_t i) const noexcept
  {
    assert(i < N);
    return m_values[i];
  }

  template <typename T, std::size_t N>
  constexpr bool vec<T, N>::operator==(const vec& other) const noexcept
  {
    int i = 0;
    while (i < N && m_values[i] == other.m_values[i])
      i++;
    return i == N;
  }

  template <typename T, std::size_t N>
  constexpr bool vec<T, N>::operator!=(const vec& other) const noexcept
  {
    return !(*this == other);
  }
} // namespace zagan