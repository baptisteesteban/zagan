#pragma once

#include <zagan/core/domains/box3d.hpp>
#include <zagan/core/points/point3d.hpp>

#include <cassert>
#include <memory>
#include <utility>

namespace zagan
{
  template <typename T>
  class image3d
  {
  public:
    using value_t  = T;
    using domain_t = box3d;
    using point_t  = domain_t::point_t;

  public:
    image3d() noexcept;
    image3d(int width, int height, int depth);

    image3d(const image3d&) noexcept;
    image3d(image3d&&) noexcept;

    image3d& operator=(const image3d&) noexcept;
    image3d& operator=(image3d&&) noexcept;

    int width() const noexcept;
    int height() const noexcept;
    int depth() const noexcept;

    domain_t domain() const noexcept;

    T*       buffer() noexcept;
    const T* buffer() const noexcept;

    T&       operator()(int x, int y, int z) noexcept;
    const T& operator()(int x, int y, int z) const noexcept;

    T&       operator()(const point_t&) noexcept;
    const T& operator()(const point_t&) const noexcept;

  private:
    std::shared_ptr<T[]> m_data;
    int                  m_width;
    int                  m_height;
    int m_depth;
  };

  /*
   * Implementation
   */

  template <typename T>
  image3d<T>::image3d() noexcept
    : m_data(nullptr)
    , m_width(0)
    , m_height(0)
    , m_depth(0)
  {
  }

  template <typename T>
  image3d<T>::image3d(int width, int height, int depth)
    : m_data(std::make_shared<T[]>(width * height * depth))
    , m_width(width)
    , m_height(height)
    , m_depth(depth)
  {
  }

  template <typename T>
  image3d<T>::image3d(const image3d& other) noexcept
    : m_data(other.m_data)
    , m_width(other.m_width)
    , m_height(other.m_height)
    , m_depth(other.m_depth)
  {
  }

  template <typename T>
  image3d<T>::image3d(image3d&& other) noexcept
    : m_data(std::exchange(other.m_data, nullptr))
    , m_width(std::exchange(other.m_width, 0))
    , m_height(std::exchange(other.m_height, 0))
    , m_depth(std::exchange(other.m_depth, 0))
  {
  }

  template <typename T>
  image3d<T>& image3d<T>::operator=(const image3d& other) noexcept
  {
    m_data   = other.m_data;
    m_width  = other.m_width;
    m_height = other.m_height;
    m_depth  = other.m_depth;
    return *this;
  }

  template <typename T>
  image3d<T>& image3d<T>::operator=(image3d&& other) noexcept
  {
    m_data   = std::exchange(other.m_data, nullptr);
    m_width  = std::exchange(other.m_width, 0);
    m_height = std::exchange(other.m_height, 0);
    m_depth  = std::exchange(other.m_depth, 0);
    return *this;
  }

  template <typename T>
  inline int image3d<T>::width() const noexcept
  {
    return m_width;
  }

  template <typename T>
  inline int image3d<T>::height() const noexcept
  {
    return m_height;
  }

  template <typename T>
  inline int image3d<T>::depth() const noexcept
  {
    return m_depth;
  }

  template <typename T>
  typename image3d<T>::domain_t image3d<T>::domain() const noexcept
  {
    return domain_t{static_cast<std::size_t>(m_width), static_cast<std::size_t>(m_height), static_cast<std::size_t>(m_depth)};
  }

  template <typename T>
  inline T* image3d<T>::buffer() noexcept
  {
    return m_data.get();
  }

  template <typename T>
  inline const T* image3d<T>::buffer() const noexcept
  {
    return m_data.get();
  }

  template <typename T>
  T& image3d<T>::operator()(int x, int y, int z) noexcept
  {
    assert(x >= 0 && y >= 0 && z >= 0 && x < m_width && y < m_height && z < m_depth);
    return m_data[z * m_width * m_height + y * m_width + x];
  }

  template <typename T>
  const T& image3d<T>::operator()(int x, int y, int z) const noexcept
  {
    assert(x >= 0 && y >= 0 && z >= 0 && x < m_width && y < m_height && z < m_depth);
    return m_data[z * m_width * m_height + y * m_width + x];
  }

  template <typename T>
  inline T& image3d<T>::operator()(const typename image3d<T>::point_t& p) noexcept
  {
    return this->operator()(p[0], p[1], p[2]);
  }

  template <typename T>
  inline const T& image3d<T>::operator()(const typename image3d<T>::point_t& p) const noexcept
  {
    return this->operator()(p[0], p[1], p[2]);
  }
} // namespace zagan