#pragma once

#include <cassert>
#include <cstring>
namespace zagan
{
  template <unsigned N, typename T>
  class ndpoint
  {
  public:
    ndpoint() noexcept;
    ndpoint(T x) noexcept
      requires(N == 1);
    ndpoint(T x, T y) noexcept
      requires(N == 2);
    ndpoint(const ndpoint&) noexcept;
    ndpoint& operator=(const ndpoint&) noexcept;

    const T& x() const noexcept;
    const T& y() const noexcept
      requires(N >= 2);

    T&       operator[](unsigned i) noexcept;
    const T& operator[](unsigned i) const noexcept;

  public:
    static constexpr unsigned ndim() noexcept { return N; };

  private:
    int m_values[N];
  };

  /*
   * Implementation
   */

  template <unsigned N, typename T>
  ndpoint<N, T>::ndpoint() noexcept
  {
    std::memset(m_values, 0, 2 * sizeof(T));
  }

  template <unsigned N, typename T>
  ndpoint<N, T>::ndpoint(T x) noexcept
    requires(N == 1)
  {
    m_values[0] = x;
  }

  template <unsigned N, typename T>
  ndpoint<N, T>::ndpoint(T x, T y) noexcept
    requires(N == 2)
  {
    m_values[0] = x;
    m_values[1] = y;
  }

  template <unsigned N, typename T>
  ndpoint<N, T>::ndpoint(const ndpoint& other) noexcept
  {
    std::memcpy((void*)other.m_values, m_values, N * sizeof(T));
  }

  template <unsigned N, typename T>
  ndpoint<N, T>& ndpoint<N, T>::operator=(const ndpoint& other) noexcept
  {
    std::memcpy((void*)other.m_values, m_values, N * sizeof(T));
    return *this;
  }

  template <unsigned N, typename T>
  const T& ndpoint<N, T>::x() const noexcept
  {
    return m_values[0];
  }

  template <unsigned N, typename T>
  const T& ndpoint<N, T>::y() const noexcept
    requires(N >= 2)
  {
    return m_values[1];
  }

  template <unsigned N, typename T>
  T& ndpoint<N, T>::operator[](unsigned i) noexcept
  {
    assert(i < N);
    return m_values[i];
  }

  template <unsigned N, typename T>
  const T& ndpoint<N, T>::operator[](unsigned i) const noexcept
  {
    assert(i < N);
    return m_values[i];
  }
} // namespace zagan