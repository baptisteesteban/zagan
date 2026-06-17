#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>

#include <zagan/core/image/image2d.hpp>
#include <zagan/core/interpolation/bounds.hpp>
#include <zagan/core/interpolation/linear.hpp>
#include <zagan/core/points/vec3.hpp>
#include <zagan/core/volume/volume.hpp>
#include <zagan/cpr/centerline.hpp>

namespace zagan::cpr
{
  template <typename V>
  image2d<V> stretched(const centerline& cl, const volume<V>& vol, vec3 direction, int half_width,
                       double pixel_spacing_mm)
  {
    assert(half_width >= 0);
    assert(pixel_spacing_mm > 0.0);

    const std::size_t n = cl.size();
    if (n < 2)
      return image2d<V>{};

    const vec3 D = normalized(direction);

    std::vector<double> y(n, 0.0);
    for (std::size_t i = 0; i + 1 < n; ++i)
    {
      const vec3   d     = cl.points()[i + 1] - cl.points()[i];
      const double dl    = dot(d, D);
      const double dd    = dot(d, d);
      const double delta = std::sqrt(std::max(0.0, dd - dl * dl));
      y[i + 1]           = y[i] + delta;
    }

    const double total_y = y.back();
    const int    width   = 2 * half_width + 1;
    const int    height  = static_cast<int>(std::ceil(total_y / pixel_spacing_mm)) + 1;

    linear_interpolator<bounds::clamp> interp{};
    image2d<V>                         out(width, height);

    std::size_t seg = 0;
    for (int row = 0; row < height; ++row)
    {
      const double yr = std::min(static_cast<double>(row) * pixel_spacing_mm, total_y);

      while (seg + 2 < n && y[seg + 1] < yr)
        ++seg;

      const double dy = y[seg + 1] - y[seg];
      const double t  = (dy > 0.0) ? (yr - y[seg]) / dy : 0.0;
      const vec3   P  = cl.points()[seg] + (cl.points()[seg + 1] - cl.points()[seg]) * t;

      for (int u = -half_width; u <= half_width; ++u)
      {
        const vec3 world         = P + D * (static_cast<double>(u) * pixel_spacing_mm);
        out(u + half_width, row) = interp(vol, world);
      }
    }

    return out;
  }

} // namespace zagan::cpr
