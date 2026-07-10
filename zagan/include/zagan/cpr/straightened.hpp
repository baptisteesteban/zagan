#pragma once

#include <cmath>
#include <zagan/core/image/image2d.hpp>
#include <zagan/core/image/image3d.hpp>
#include <zagan/core/volume/volume.hpp>
#include <zagan/cpr/centerline.hpp>
#include <zagan/cpr/frame.hpp>

#include <vector>

namespace zagan::cpr
{
  struct straightened_params
  {
    double width_mm;
    int    columns;
    int    num_angles;
  };

  template <typename T, typename Interp>
  inline image3d<T> straighten_mpr(const volume<T>& vol, const centerline& cl, const straightened_params& params,
                                   const Interp& interp)
  {
    centerline         resampled_cl = cl.resampled(params.width_mm / params.columns);
    std::vector<frame> frames       = rotation_minimizing_frame(resampled_cl);

    if (frames.empty())
      return image3d<T>(params.columns, 0, 0);

    image3d<T> out(params.columns, static_cast<int>(resampled_cl.size()), params.num_angles);

    for (int a = 0; a < params.num_angles; ++a)
    {
      double theta = a * (M_PI / params.num_angles);
      double ct    = std::cos(theta);
      double st    = std::sin(theta);
      for (std::size_t i = 0; i < resampled_cl.size(); ++i)
      {
        const vec3&  center_world = resampled_cl.points()[i];
        const frame& f            = frames[i];
        const vec3   dir          = f.u * ct + f.v * st;

        for (int x = 0; x < params.columns; ++x)
        {
          double offset_mm = (x - params.columns / 2.0) * params.width_mm / params.columns;
          vec3   sample_pt = center_world + dir * offset_mm;

          out(x, static_cast<int>(i), a) = interp(vol, sample_pt);
        }
      }
    }
    return out;
  }
} // namespace zagan::cpr