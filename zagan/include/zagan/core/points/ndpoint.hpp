#pragma once

#include <cassert>
#include <cstring>
namespace zagan
{
  template <std::size_t N, typename T>
  class ndpoint
  {
  public:
    constexpr ndpoint() noexcept;
    constexpr ndpoint(T x) noexcept
      requires(N == 1);
    constexpr ndpoint(T x, T y) noexcept
      requires(N == 2);
    constexpr ndpoint(const ndpoint&) noexcept;
    constexpr ndpoint& operator=(const ndpoint&) noexcept;

    constexpr const T& x() const noexcept;
    constexpr const T& y() const noexcept
      requires(N >= 2);

    constexpr T&       operator[](std::size_t i) noexcept;
    constexpr const T& operator[](std::size_t i) const noexcept;

  public:
    static constexpr std::size_t ndim() noexcept { return N; };

  private:
    int m_values[N];
  };

  template <std::size_t N, typename T>
  constexpr bool operator==(const ndpoint<N, T>& p, const ndpoint<N, T>& q) noexcept;

  template <std::size_t N, typename T>
  constexpr bool operator!=(const ndpoint<N, T>& p, const ndpoint<N, T>& q) noexcept;

  /*
   * Implementation
   */

  template <std::size_t N, typename T>
  constexpr ndpoint<N, T>::ndpoint() noexcept
  {
    for (int i = 0; i < N; i++)
      m_values[i] = T();
  }

  template <std::size_t N, typename T>
  constexpr ndpoint<N, T>::ndpoint(T x) noexcept
    requires(N == 1)
  {
    m_values[0] = x;
  }

  template <std::size_t N, typename T>
  constexpr ndpoint<N, T>::ndpoint(T x, T y) noexcept
    requires(N == 2)
  {
    m_values[0] = x;
    m_values[1] = y;
  }

  template <std::size_t N, typename T>
  constexpr ndpoint<N, T>::ndpoint(const ndpoint& other) noexcept
  {
    std::memcpy((void*)other.m_values, m_values, N * sizeof(T));
  }

  template <std::size_t N, typename T>
  constexpr ndpoint<N, T>& ndpoint<N, T>::operator=(const ndpoint& other) noexcept
  {
    std::memcpy((void*)other.m_values, m_values, N * sizeof(T));
    return *this;
  }

  template <std::size_t N, typename T>
  constexpr const T& ndpoint<N, T>::x() const noexcept
  {
    return m_values[0];
  }

  template <std::size_t N, typename T>
  constexpr const T& ndpoint<N, T>::y() const noexcept
    requires(N >= 2)
  {
    return m_values[1];
  }

  template <std::size_t N, typename T>
  constexpr T& ndpoint<N, T>::operator[](std::size_t i) noexcept
  {
    assert(i < N);
    return m_values[i];
  }

  template <std::size_t N, typename T>
  constexpr const T& ndpoint<N, T>::operator[](std::size_t i) const noexcept
  {
    assert(i < N);
    return m_values[i];
  }

  template <std::size_t N, typename T>
  constexpr bool operator==(const ndpoint<N, T>& p, const ndpoint<N, T>& q) noexcept
  {
    int i = 0;
    while (i < N && p[i] == q[i])
      i++;
    return i == N;
  }

  template <std::size_t N, typename T>
  constexpr bool operator!=(const ndpoint<N, T>& p, const ndpoint<N, T>& q) noexcept
  {
    return !(p == q);
  }
} // namespace zagan