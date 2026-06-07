#pragma once

#include <zagan/core/vec.hpp>

#include <cstdint>

namespace zagan::colors
{
  template <typename T>
  class rgb;

  using rgb8 = rgb<std::uint8_t>;

  template <typename T>
  class rgb : public vec<T, 3>
  {
  public:
    using vec<T, 3>::vec;

    rgb(const T& r, const T& g, const T& b) noexcept;
    rgb(T values[3]) noexcept;

    T&       red() noexcept;
    const T& red() const noexcept;
    T&       green() noexcept;
    const T& green() const noexcept;
    T&       blue() noexcept;
    const T& blue() const noexcept;
  };

  /*
   * Implementation
   */
  template <typename T>
  rgb<T>::rgb(const T& r, const T& g, const T& b) noexcept
  {
    this->m_values[0] = r;
    this->m_values[1] = g;
    this->m_values[2] = b;
  }

  template <typename T>
  rgb<T>::rgb(T values[3]) noexcept
  {
    std::memcpy(this->m_values, values, 3 * sizeof(T));
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