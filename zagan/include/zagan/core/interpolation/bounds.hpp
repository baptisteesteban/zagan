#pragma once

#include <zagan/core/points/point3d.hpp>

#include <algorithm>

namespace zagan
{
  namespace bounds
  {
    struct clamp
    {
      template <typename Vol>
      static typename Vol::value_t get(const Vol& v, const point3d& p) noexcept
      {
        int x = std::clamp(p.x(), 0, v.width() - 1);
        int y = std::clamp(p.y(), 0, v.height() - 1);
        int z = std::clamp(p.z(), 0, v.depth() - 1);
        return v(x, y, z);
      }
    };

    template <typename T>
    struct padding
    {
      T m_pad_value;

      constexpr padding(T pad_value = T()) noexcept
        : m_pad_value(pad_value)
      {
      }

      template <typename Vol>
      typename Vol::value_t get(const Vol& v, const point3d& p) const noexcept
      {
        if (p.x() < 0 || p.x() >= v.width() || p.y() < 0 || p.y() >= v.height() || p.z() < 0 || p.z() >= v.depth())
        {
          return static_cast<typename Vol::value_t>(m_pad_value);
        }
        return v(p);
      }
    };
  } // namespace bounds
} // namespace zagan