#pragma once

#include <cassert>
#include <zagan/core/points/ndpoint.hpp>

namespace zagan
{
  template <std::size_t N, typename T>
  class ndbox
  {
  public:
    using point_t = ndpoint<N, T>;

  public:
    constexpr ndbox() noexcept = default;
    constexpr ndbox(std::size_t width) noexcept
      requires(N == 1);
    constexpr ndbox(std::size_t width, std::size_t height) noexcept
      requires(N == 2);
    constexpr ndbox(std::size_t width, std::size_t height, std::size_t depth) noexcept
      requires(N == 3);
    constexpr ndbox(const point_t& pmin, const point_t& pmax) noexcept;

    constexpr T shape(std::size_t i) const noexcept;

    constexpr const point_t& pmin() const noexcept;
    constexpr const point_t& pmax() const noexcept;

    constexpr bool has(const point_t&) const noexcept;

  private:
    point_t m_pmin;
    point_t m_pmax; // Outside
  };

  /*
   * Implementation
   */

  template <std::size_t N, typename T>
  constexpr ndbox<N, T>::ndbox(std::size_t width) noexcept
    requires(N == 1)
    : m_pmin()
    , m_pmax(width)
  {
  }

  template <std::size_t N, typename T>
  constexpr ndbox<N, T>::ndbox(std::size_t width, std::size_t height) noexcept
    requires(N == 2)
    : m_pmin()
    , m_pmax(width, height)
  {
  }

  template <std::size_t N, typename T>
  constexpr ndbox<N, T>::ndbox(std::size_t width, std::size_t height, std::size_t depth) noexcept
    requires(N == 3)
    : m_pmin()
    , m_pmax(width, height, depth)
  {
  }

  template <std::size_t N, typename T>
  constexpr ndbox<N, T>::ndbox(const typename ndbox<N, T>::point_t& pmin,
                               const typename ndbox<N, T>::point_t& pmax) noexcept
    : m_pmin(pmin)
    , m_pmax(pmax)
  {
  }

  template <std::size_t N, typename T>
  constexpr T ndbox<N, T>::shape(std::size_t i) const noexcept
  {
    assert(i < N);
    return m_pmax[i] - m_pmin[i];
  }

  template <std::size_t N, typename T>
  inline constexpr const typename ndbox<N, T>::point_t& ndbox<N, T>::pmin() const noexcept
  {
    return m_pmin;
  }

  template <std::size_t N, typename T>
  inline constexpr const typename ndbox<N, T>::point_t& ndbox<N, T>::pmax() const noexcept
  {
    return m_pmax;
  }

  template <std::size_t N, typename T>
  constexpr bool ndbox<N, T>::has(const typename ndbox<N, T>::point_t& p) const noexcept
  {
    int i = 0;
    while (i < N && p[i] >= m_pmin[i] && p[i] < m_pmax[i])
      i++;
    return i == N;
  }
} // namespace zagan