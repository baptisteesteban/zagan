#pragma once

#include <zagan/colors/rgb.hpp>
#include <zagan/core/image/image2d.hpp>
#include <zagan/core/points/point2d.hpp>

#include <cmath>
#include <iostream>
#include <memory>

namespace zagan::labeling
{
  namespace details
  {
    struct slic_cluster
    {
      std::uint8_t r;
      std::uint8_t g;
      std::uint8_t b;
      int          x;
      int          y;
    };
  } // namespace details

  template <typename L>
  image2d<L> slic(const image2d<colors::rgb8>& img, int K = 100, int compactness = 10, int iter = 10)
  {
    image2d<L> res(img.width(), img.height());

    auto cluster_centers = std::make_unique<details::slic_cluster[]>(K);

    // 1. Cluster initialization
    const int spacing = std::sqrt((img.width() * img.height()) / K);
    {
      int cnt = 0;
      for (int y = spacing / 2; y < img.height(); y += spacing)
      {
        for (int x = spacing / 2; x < img.width(); x += spacing)
        {
          cluster_centers[cnt].x = x;
          cluster_centers[cnt].y = y;
          cluster_centers[cnt].r = img(x, y).red();
          cluster_centers[cnt].g = img(x, y).green();
          cluster_centers[cnt].b = img(x, y).blue();
          cnt += 1;
        }
      }
      std::cout << "Counter: " << cnt << "\n";
    }

    auto compute_dist = [ratio = compactness / spacing, &img](const details::slic_cluster& c1, const point2d& p) {
      auto x2 = (c1.x - p.x());
      x2 *= x2;
      auto y2 = (c1.y - p.y());
      y2 *= y2;
      const auto dxy = std::sqrt(x2 + y2);

      const auto v  = img(p);
      auto       r2 = (c1.r - v.red());
      r2 *= r2;
      auto g2 = (c1.g - v.green());
      g2 *= g2;
      auto b2 = (c1.b - v.blue());
      b2 *= b2;
      const auto drgb = std::sqrt(r2 + g2 + b2);
      return drgb + ratio * dxy;
    };
    // 2. Iterations
    for (int it = 0; it < iter; it++)
    {
      // 2.1 Compute cluster for each point
      for (int y = 0; y < img.height(); y++)
      {
        for (int x = 0; x < img.width(); x++)
        {
          const auto cur_p = point2d{x, y};
          auto       min_d = compute_dist(cluster_centers[res(x, y)], cur_p);
          for (int c = 0; c < K; c++)
          {
            auto d = compute_dist(cluster_centers[c], cur_p);
            if (d < min_d)
            {
              min_d      = d;
              res(cur_p) = c;
            }
          }
        }
      }

      // 2.2 Update cluster centers
      auto sum_p = std::make_unique<point2d[]>(K);
      auto sum_v = std::make_unique<colors::rgb<std::uint16_t>[]>(K);
      auto area  = std::make_unique<int[]>(K);
      for (int y = 0; y < img.height(); y++)
      {
        for (int x = 0; x < img.width(); x++)
        {
          const auto lbl = res(x, y);
          sum_p[lbl] += point2d{x, y};
          sum_v[lbl] += img(x, y);
          area[lbl]++;
        }
      }
      for (int c = 0; c < K; c++)
      {
        if (area[c] == 0)
          continue;
        const auto p_mean    = sum_p[c] / area[c];
        const auto v_mean    = sum_v[c] / area[c];
        cluster_centers[c].x = p_mean[0];
        cluster_centers[c].y = p_mean[1];
        cluster_centers[c].r = v_mean[0];
        cluster_centers[c].g = v_mean[1];
        cluster_centers[c].b = v_mean[2];
      }
    }

    return res;
  }
} // namespace zagan::labeling