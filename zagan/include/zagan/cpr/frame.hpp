#pragma once

#include <zagan/core/points/vec3.hpp>
#include <zagan/cpr/centerline.hpp>

#include <vector>

namespace zagan::cpr
{
  struct frame
  {
    vec3 t, u, v;
  };

  inline std::vector<frame> rotation_minimizing_frame(const centerline& cl)
  {
    std::vector<frame> frames;
    if (cl.size() < 2)
      return frames;

    const std::vector<vec3>& p = cl.points();

    vec3 t = cl.tangent(0);
    // initial reference vector, not parallel to the tangent
    vec3 u = cross(t, vec3(0, 1, 0));
    if (u == vec3(0, 0, 0))
    {
      u = cross(t, vec3(0, 0, 1));
    }
    u      = normalized(u);
    vec3 v = cross(t, u);

    frames.push_back({t, u, v});

    // double reflection method
    for (std::size_t i = 0; i + 1 < cl.size(); ++i)
    {
      vec3   c1   = p[i + 1] - p[i];
      double c1c1 = dot(c1, c1);

      // reflect the previous frame onto the next segment
      vec3 tL = t - 2 * dot(c1, t) / c1c1 * c1;
      vec3 uL = u - 2 * dot(c1, u) / c1c1 * c1;

      t = cl.tangent(i + 1);

      vec3   c2   = t - tL;
      double c2c2 = dot(c2, c2);

      u = uL - 2 * dot(c2, uL) / c2c2 * c2;
      v = cross(t, u);

      frames.push_back({t, u, v});
    }

    return frames;
  }

} // namespace zagan::cpr