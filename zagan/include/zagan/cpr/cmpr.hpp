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
  image2d<V> stretched(const centerline& ct, const volume<V>& volume, vec3 direction, int half_width,
                       double pixel_spacing_mm)
  {
    assert(half_width >= 0);
    assert(pixel_spacing_mm > 0.0);

    const std::size_t nb_points = ct.size();
    if (nb_points < 2)
      return image2d<V>{};

    const vec3 line_of_interest = normalized(direction);

    // accumulate perpendicular arc length along the ct
    // delta_i = sqrt(|segment|^2 - (segment . loi)^2)
    std::vector<double> p_arc(nb_points, 0.0);
    for (std::size_t i = 0; i + 1 < nb_points; ++i)
    {
      const vec3   segment = ct.points()[i + 1] - ct.points()[i];
      const double par_len = dot(segment, line_of_interest);
      const double perp_sq = dot(segment, segment) - par_len * par_len;
      p_arc[i + 1]         = p_arc[i] + std::sqrt(std::max(perp_sq, 0.0));
    }
    const double total_p_arc = p_arc.back();

    // compute image bounds from the ct's extent along the line of interest
    std::vector<double> projection_loi(nb_points);
    for (std::size_t i = 0; i < nb_points; ++i)
      projection_loi[i] = dot(ct.points()[i], line_of_interest);

    const double loi_min = *std::min_element(projection_loi.begin(), projection_loi.end()) -
                           static_cast<double>(half_width) * pixel_spacing_mm;
    const double loi_max = *std::max_element(projection_loi.begin(), projection_loi.end()) +
                           static_cast<double>(half_width) * pixel_spacing_mm;

    const int w = static_cast<int>(std::ceil((loi_max - loi_min) / pixel_spacing_mm)) + 1;
    const int h = static_cast<int>(std::ceil(total_p_arc / pixel_spacing_mm)) + 1;

    // for each row find the vessel position at that perp arc length,
    // then sample the volume along the line of interest for every column
    linear_interpolator<bounds::clamp> interpolator{};
    image2d<V>                         output(w, h);

    std::size_t curr_seg = 0;
    for (int y = 0; y < h; ++y)
    {
      const double target_parc = std::min(static_cast<double>(y) * pixel_spacing_mm, total_p_arc);

      while (curr_seg + 2 < nb_points && p_arc[curr_seg + 1] < target_parc)
        ++curr_seg;

      const double seg_parc     = p_arc[curr_seg + 1] - p_arc[curr_seg];
      const double t            = (seg_parc > 0.0) ? (target_parc - p_arc[curr_seg]) / seg_parc : 0.0;
      const vec3   vessel_point = ct.points()[curr_seg] + (ct.points()[curr_seg + 1] - ct.points()[curr_seg]) * t;

      for (int x = 0; x < w; ++x)
      {
        const double col_loi      = loi_min + static_cast<double>(x) * pixel_spacing_mm;
        const vec3   sample_point = vessel_point + line_of_interest * (col_loi - dot(vessel_point, line_of_interest));
        output(x, y)              = interpolator(volume, sample_point);
      }
    }

    return output;
  }

} // namespace zagan::cpr
