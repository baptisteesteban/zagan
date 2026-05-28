#pragma once

#include <cassert>
#include <cmath>
#include <vector>
#include <zagan/core/points/vec3.hpp>

namespace zagan::cpr
{
  class centerline
  {
  public:
    centerline() = default;
    explicit centerline(std::vector<vec3> points, std::vector<double> radii = {}) noexcept;

    double     arc_length() const noexcept;
    centerline resampled(double step_mm) const;
    vec3       tangent(std::size_t i) const noexcept;

    const std::vector<vec3>&   points() const noexcept;
    const std::vector<double>& radii() const noexcept;
    std::size_t                size() const noexcept;

  private:
    std::vector<vec3>   m_points;
    std::vector<double> m_radii;
  };

  /*
   * Implementation
   */

  inline centerline::centerline(std::vector<vec3> points, std::vector<double> radii) noexcept
    : m_points(std::move(points))
    , m_radii(std::move(radii))
  {
  }

  inline double centerline::arc_length() const noexcept
  {
    double length = 0.0;
    for (std::size_t i = 1; i < m_points.size(); ++i)
    {
      length += norm(m_points[i] - m_points[i - 1]);
    }
    return length;
  }

  inline vec3 centerline::tangent(std::size_t i) const noexcept
  {
    assert(i < m_points.size());
    if (m_points.size() < 2)
    {
      return vec3(0.0, 0.0, 0.0);
    }

    if (i == 0)
    {
      return normalized(m_points[1] - m_points[0]);
    }
    if (i == m_points.size() - 1)
    {
      return normalized(m_points[i] - m_points[i - 1]);
    }

    return normalized(m_points[i + 1] - m_points[i - 1]);
  }

  inline const std::vector<vec3>& centerline::points() const noexcept
  {
    return m_points;
  }

  inline const std::vector<double>& centerline::radii() const noexcept
  {
    return m_radii;
  }

  inline std::size_t centerline::size() const noexcept
  {
    return m_points.size();
  }

  inline centerline centerline::resampled(double step_mm) const
  {
    assert(step_mm > 0.0);
    if (m_points.empty())
      return centerline{};
    if (m_points.size() == 1)
      return centerline{m_points, m_radii};

    std::vector<vec3>   new_points;
    std::vector<double> new_radii;
    bool                has_radii = !m_radii.empty();

    new_points.push_back(m_points.front());
    if (has_radii)
      new_radii.push_back(m_radii.front());

    double accumulated_dist = 0.0;

    for (std::size_t i = 0; i < m_points.size() - 1; ++i)
    {
      vec3   p0 = m_points[i];
      vec3   p1 = m_points[i + 1];
      double r0 = has_radii ? m_radii[i] : 0.0;
      double r1 = has_radii ? m_radii[i + 1] : 0.0;

      double segment_length = norm(p1 - p0);
      vec3   dir            = p1 - p0;
      if (segment_length > 0)
      {
        dir = dir * (1.0 / segment_length);
      }

      while (accumulated_dist + step_mm <= segment_length)
      {
        accumulated_dist += step_mm;
        double t = accumulated_dist / segment_length;

        new_points.push_back(p0 + dir * accumulated_dist);
        if (has_radii)
        {
          new_radii.push_back(r0 * (1.0 - t) + r1 * t);
        }
      }

      accumulated_dist -= segment_length;
    }

    if (norm(new_points.back() - m_points.back()) > 1e-6)
    {
      new_points.push_back(m_points.back());
      if (has_radii)
        new_radii.push_back(m_radii.back());
    }

    return centerline{std::move(new_points), std::move(new_radii)};
  }
} // namespace zagan::cpr