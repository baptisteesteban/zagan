#pragma once

#include <zagan/core/interpolation/bounds.hpp>
#include <zagan/core/points/point3d.hpp>
#include <zagan/core/points/vec3.hpp>

#include <cmath>

namespace zagan
{
  template <typename BoundsPolicy = bounds::clamp>
  struct nearest_interpolator
  {
    BoundsPolicy m_bounds;

    constexpr nearest_interpolator(BoundsPolicy bounds = BoundsPolicy()) noexcept
      : m_bounds(bounds)
    {
    }

    template <typename Vol>
    typename Vol::value_t operator()(const Vol& v, const vec3& world_pos) const noexcept
    {
      vec3 voxel_pos = v.to_voxel(world_pos);

      int x = static_cast<int>(std::round(voxel_pos.x()));
      int y = static_cast<int>(std::round(voxel_pos.y()));
      int z = static_cast<int>(std::round(voxel_pos.z()));

      return m_bounds.get(v, point3d(x, y, z));
    }
  };
} // namespace zagan