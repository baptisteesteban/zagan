#pragma once

#include <cassert>
#include <memory>
#include <utility>

namespace zagan
{
  template <typename T>
  class image2d
  {
  public:
    image2d() noexcept;
    image2d(int width, int height);
    image2d(const image2d&) noexcept;
    image2d(image2d&&) noexcept;
    image2d& operator=(const image2d&) noexcept;
    image2d& operator=(image2d&&) noexcept;

    int width() const noexcept;
    int height() const noexcept;

    T*       buffer() noexcept;
    const T* buffer() const noexcept;

    T&       operator()(int x, int y) noexcept;
    const T& operator()(int x, int y) const noexcept;

  private:
    std::shared_ptr<T[]> m_data;
    int                  m_width;
    int                  m_height;
  };

  /*
   * Implementation
   */

  template <typename T>
  image2d<T>::image2d() noexcept
    : m_data(nullptr)
    , m_width(0)
    , m_height(0)
  {
  }

  template <typename T>
  image2d<T>::image2d(int width, int height)
    : m_data(std::make_shared<T[]>(width * height))
    , m_width(width)
    , m_height(height)
  {
  }

  template <typename T>
  image2d<T>::image2d(const image2d& other) noexcept
    : m_data(other.m_data)
    , m_width(other.m_width)
    , m_height(other.m_height)
  {
  }

  template <typename T>
  image2d<T>::image2d(image2d&& other) noexcept
    : m_data(std::exchange(other.m_data, nullptr))
    , m_width(std::exchange(other.m_width, 0))
    , m_height(std::exchange(other.m_height, 0))
  {
  }

  template <typename T>
  image2d<T>& image2d<T>::operator=(const image2d& other) noexcept
  {
    m_data   = other.m_data;
    m_width  = other.m_width;
    m_height = other.m_height;
    return *this;
  }

  template <typename T>
  image2d<T>& image2d<T>::operator=(image2d&& other) noexcept
  {
    m_data   = std::exchange(other.m_data, nullptr);
    m_width  = std::exchange(other.m_width, 0);
    m_height = std::exchange(other.m_height, 0);
    return *this;
  }

  template <typename T>
  inline int image2d<T>::width() const noexcept
  {
    return m_width;
  }

  template <typename T>
  inline int image2d<T>::height() const noexcept
  {
    return m_height;
  }

  template <typename T>
  inline T* image2d<T>::buffer() noexcept
  {
    return m_data.get();
  }

  template <typename T>
  inline const T* image2d<T>::buffer() const noexcept
  {
    return m_data.get();
  }

  template <typename T>
  T& image2d<T>::operator()(int x, int y) noexcept
  {
    assert(x >= 0 && y >= 0 && x < m_width && y < m_height);
    return m_data[y * m_width + x];
  }

  template <typename T>
  const T& image2d<T>::operator()(int x, int y) const noexcept
  {
    assert(x >= 0 && y >= 0 && x < m_width && y < m_height);
    return m_data[y * m_width + x];
  }
} // namespace zagan