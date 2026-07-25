#pragma once

#include <zagan/core/interpolation/bounds.hpp>
#include <zagan/core/points/point3d.hpp>
#include <zagan/core/points/vec3.hpp>

#include <cmath>

namespace zagan
{
  template <typename BoundsPolicy = bounds::clamp>
  struct linear_interpolator
  {
    BoundsPolicy m_bounds;

    constexpr linear_interpolator(BoundsPolicy bounds = BoundsPolicy()) noexcept
      : m_bounds(bounds)
    {
    }

    template <typename Vol>
    typename Vol::value_t operator()(const Vol& v, const vec3& world_pos) const noexcept
    {
      vec3 voxel_pos = v.to_voxel(world_pos);

      double x = voxel_pos.x();
      double y = voxel_pos.y();
      double z = voxel_pos.z();

      int x0 = static_cast<int>(std::floor(x));
      int y0 = static_cast<int>(std::floor(y));
      int z0 = static_cast<int>(std::floor(z));

      int x1 = x0 + 1;
      int y1 = y0 + 1;
      int z1 = z0 + 1;

      double xd = x - x0;
      double yd = y - y0;
      double zd = z - z0;

      auto c000 = m_bounds.get(v, point3d(x0, y0, z0));
      auto c100 = m_bounds.get(v, point3d(x1, y0, z0));
      auto c010 = m_bounds.get(v, point3d(x0, y1, z0));
      auto c110 = m_bounds.get(v, point3d(x1, y1, z0));
      auto c001 = m_bounds.get(v, point3d(x0, y0, z1));
      auto c101 = m_bounds.get(v, point3d(x1, y0, z1));
      auto c011 = m_bounds.get(v, point3d(x0, y1, z1));
      auto c111 = m_bounds.get(v, point3d(x1, y1, z1));

      double c00 = c000 * (1.0 - xd) + c100 * xd;
      double c10 = c010 * (1.0 - xd) + c110 * xd;
      double c01 = c001 * (1.0 - xd) + c101 * xd;
      double c11 = c011 * (1.0 - xd) + c111 * xd;

      double c0 = c00 * (1.0 - yd) + c10 * yd;
      double c1 = c01 * (1.0 - yd) + c11 * yd;

      double c = c0 * (1.0 - zd) + c1 * zd;

      return static_cast<typename Vol::value_t>(c);
    }
  };
} // namespace zagan