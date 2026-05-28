#pragma once

#include <zagan/core/image/image3d.hpp>
#include <zagan/core/points/vec3.hpp>

namespace zagan
{
  template <typename T>
  class volume : public image3d<T>
  {
  public:
    using value_t = T;

    volume() noexcept;
    volume(int width, int height, int depth, const vec3& spacing = vec3(1.0, 1.0, 1.0),
           const vec3& origin = vec3(0.0, 0.0, 0.0));

    const vec3& spacing() const noexcept;
    const vec3& origin() const noexcept;

    vec3 to_world(const point3d& voxel) const noexcept;
    vec3 to_voxel(const vec3& world) const noexcept;

  private:
    vec3 m_spacing;
    vec3 m_origin;
  };

  /*
   * Implementation
   */

  template <typename T>
  volume<T>::volume() noexcept
    : image3d<T>()
    , m_spacing(1.0, 1.0, 1.0)
    , m_origin(0.0, 0.0, 0.0)
  {
  }

  template <typename T>
  volume<T>::volume(int width, int height, int depth, const vec3& spacing, const vec3& origin)
    : image3d<T>(width, height, depth)
    , m_spacing(spacing)
    , m_origin(origin)
  {
  }

  template <typename T>
  const vec3& volume<T>::spacing() const noexcept
  {
    return m_spacing;
  }

  template <typename T>
  const vec3& volume<T>::origin() const noexcept
  {
    return m_origin;
  }

  template <typename T>
  vec3 volume<T>::to_world(const point3d& voxel) const noexcept
  {
    return vec3(m_origin.x() + voxel.x() * m_spacing.x(), m_origin.y() + voxel.y() * m_spacing.y(),
                m_origin.z() + voxel.z() * m_spacing.z());
  }

  template <typename T>
  vec3 volume<T>::to_voxel(const vec3& world) const noexcept
  {
    return vec3((world.x() - m_origin.x()) / m_spacing.x(), (world.y() - m_origin.y()) / m_spacing.y(),
                (world.z() - m_origin.z()) / m_spacing.z());
  }
} // namespace zagan