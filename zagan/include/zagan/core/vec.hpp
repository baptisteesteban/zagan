#pragma once

#include <algorithm>
#include <cassert>
#include <cstring>
#include <format>
#include <initializer_list>
#include <string>
#include <utility>

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

  protected:
    T m_values[N];
  };


  template <typename T, typename U, std::size_t N>
  constexpr bool operator==(const vec<U, N>& lhs, const vec<U, N>& rhs) noexcept;

  template <typename T, typename U, std::size_t N>
  constexpr bool operator!=(const vec<U, N>& lhs, const vec<U, N>& rhs) noexcept;

  template <typename T, typename U, std::size_t N>
  constexpr vec<decltype(std::declval<T>() + std::declval<U>()), N> operator+(const vec<T, N>& lhs,
                                                                              const vec<U, N>& rhs) noexcept;

  template <typename T, typename U, std::size_t N>
  constexpr vec<decltype(std::declval<T>() - std::declval<U>()), N> operator-(const vec<T, N>& lhs,
                                                                              const vec<U, N>& rhs) noexcept;

  template <typename T, typename U, std::size_t N>
  constexpr vec<decltype(std::declval<T>() * std::declval<U>()), N> operator*(const vec<T, N>& lhs,
                                                                              const vec<U, N>& rhs) noexcept;

  template <typename T, typename U, std::size_t N>
  constexpr vec<decltype(std::declval<T>() * std::declval<U>()), N> operator*(const vec<T, N>& lhs,
                                                                              const U&         rhs) noexcept;

  template <typename T, typename U, std::size_t N>
  constexpr vec<decltype(std::declval<T>() * std::declval<U>()), N> operator*(const T&         lhs,
                                                                              const vec<U, N>& rhs) noexcept;
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

  template <typename T, typename U, std::size_t N>
  constexpr bool operator==(const vec<T, N>& lhs, const vec<U, N>& rhs) noexcept
  {
    int i = 0;
    while (i < N && lhs[i] == rhs[i])
      i++;
    return i == N;
  }

  template <typename T, typename U, std::size_t N>
  constexpr bool operator!=(const vec<T, N>& lhs, const vec<U, N>& rhs) noexcept
  {
    return !(lhs == rhs);
  }

  template <typename T, typename U, std::size_t N>
  constexpr vec<decltype(std::declval<T>() + std::declval<U>()), N> operator+(const vec<T, N>& lhs,
                                                                              const vec<U, N>& rhs) noexcept
  {
    vec<decltype(std::declval<T>() + std::declval<U>()), N> res;
    for (int i = 0; i < N; i++)
      res[i] = lhs[i] + rhs[i];
    return res;
  }

  template <typename T, typename U, std::size_t N>
  constexpr vec<decltype(std::declval<T>() - std::declval<U>()), N> operator-(const vec<T, N>& lhs,
                                                                              const vec<U, N>& rhs) noexcept
  {
    vec<decltype(std::declval<T>() - std::declval<U>()), N> res;
    for (int i = 0; i < N; i++)
      res[i] = lhs[i] - rhs[i];
    return res;
  }

  template <typename T, typename U, std::size_t N>
  constexpr vec<decltype(std::declval<T>() * std::declval<U>()), N> operator*(const vec<T, N>& lhs,
                                                                              const vec<U, N>& rhs) noexcept
  {
    vec<decltype(std::declval<T>() * std::declval<U>()), N> res;
    for (int i = 0; i < N; i++)
      res[i] = lhs[i] * rhs[i];
    return res;
  }

  template <typename T, typename U, std::size_t N>
  constexpr vec<decltype(std::declval<T>() * std::declval<U>()), N> operator*(const vec<T, N>& lhs,
                                                                              const U&         rhs) noexcept
  {
    vec<decltype(std::declval<T>() * std::declval<U>()), N> res;
    for (int i = 0; i < N; i++)
      res[i] = lhs[i] * rhs;
    return res;
  }

  template <typename T, typename U, std::size_t N>
  inline constexpr vec<decltype(std::declval<T>() * std::declval<U>()), N> operator*(const T&         lhs,
                                                                                     const vec<U, N>& rhs) noexcept
  {
    return rhs * lhs;
  }
} // namespace zagan

template <typename T, std::size_t N>
  requires(std::formattable<T, char>)
struct std::formatter<zagan::vec<T, N>> : std::formatter<std::string>
{
  auto format(const zagan::vec<T, N>& v, std::format_context& ctx) const
  {
    std::string res = "(";
    for (int i = 0; i < N - 1; i++)
      res += std::format("{}, ", v[i]);
    res += std::format("{})", v[N - 1]);
    return std::formatter<std::string>::format(res, ctx);
  }
};