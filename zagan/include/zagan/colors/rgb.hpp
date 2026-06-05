#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>

namespace zagan::colors
{
  template <typename T>
  class rgb;

  using rgb8 = rgb<std::uint8_t>;

  template <typename T>
  class rgb
  {
  public:
    rgb() noexcept;
    rgb(const T& r, const T& g, const T& b) noexcept;
    rgb(T values[3]) noexcept;

    T&       operator[](std::size_t i) noexcept;
    const T& operator[](std::size_t i) const noexcept;

    T&       red() noexcept;
    const T& red() const noexcept;
    T&       green() noexcept;
    const T& green() const noexcept;
    T&       blue() noexcept;
    const T& blue() const noexcept;

  private:
    T m_values[3];
  };

  /*
   * Implementation
   */
  template <typename T>
  rgb<T>::rgb() noexcept
  {
    m_values[0] = T{};
    m_values[1] = T{};
    m_values[2] = T{};
  }

  template <typename T>
  rgb<T>::rgb(const T& r, const T& g, const T& b) noexcept
  {
    m_values[0] = r;
    m_values[1] = g;
    m_values[2] = b;
  }

  template <typename T>
  rgb<T>::rgb(T values[3]) noexcept
  {
    std::memcpy(m_values, values, 3 * sizeof(T));
  }

  template <typename T>
  inline T& rgb<T>::operator[](std::size_t i) noexcept
  {
    assert(i < 3);
    return m_values[i];
  }

  template <typename T>
  inline const T& rgb<T>::operator[](std::size_t i) const noexcept
  {
    assert(i < 3);
    return m_values[i];
  }

  template <typename T>
  inline T& rgb<T>::red() noexcept
  {
    return this->operator[](0);
  }

  template <typename T>
  inline const T& rgb<T>::red() const noexcept
  {
    return this->operator[](0);
  }

  template <typename T>
  inline T& rgb<T>::green() noexcept
  {
    return this->operator[](1);
  }

  template <typename T>
  inline const T& rgb<T>::green() const noexcept
  {
    return this->operator[](1);
  }

  template <typename T>
  inline T& rgb<T>::blue() noexcept
  {
    return this->operator[](2);
  }

  template <typename T>
  inline const T& rgb<T>::blue() const noexcept
  {
    return this->operator[](2);
  }
} // namespace zagan::colors