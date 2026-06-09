#pragma once

#include <limits>
#include <memory>
#include <zagan/colors/rgb.hpp>
#include <zagan/core/image/image2d.hpp>
#include <zagan/core/points/point2d.hpp>

#include <cmath>

namespace zagan::labeling
{
  namespace impl
  {
    template <typename L>
    class SLICImplementation
    {
    public:
      SLICImplementation(const image2d<colors::rgb8>& img, int K, int compactness, int iter)
        : m_img(img)
        , m_label(img.width(), img.height())
        , m_cx(std::make_unique<int[]>(img.width() * img.height()))
        , m_cy(std::make_unique<int[]>(img.width() * img.height()))
        , m_cr(std::make_unique<std::uint8_t[]>(img.width() * img.height()))
        , m_cg(std::make_unique<std::uint8_t[]>(img.width() * img.height()))
        , m_cb(std::make_unique<std::uint8_t[]>(img.width() * img.height()))
        , m_K(K)
        , m_compactness(compactness)
        , m_iter(iter)
        , m_spacing(std::sqrt(img.width() * img.height() / K))
      {
      }

      void execute()
      {
        initialize_clusters();
        for (int it = 0; it < m_iter; ++it)
          iter();
      }

      const image2d<L>& label() const noexcept { return m_label; }

    private:
      void initialize_clusters() noexcept
      {
        int cur = 0;
        for (int y = m_spacing / 2; y < m_img.height(); y += m_spacing)
        {
          for (int x = m_spacing / 2; x < m_img.width(); x += m_spacing)
          {
            m_cx[cur]     = x;
            m_cy[cur]     = y;
            m_cr[cur]     = m_img(x, y).red();
            m_cg[cur]     = m_img(x, y).green();
            m_cb[cur]     = m_img(x, y).blue();
            m_label(x, y) = cur++;
          }
        }
      }

      void iter() noexcept
      {
        // Initialize distance map for current iteration
        image2d<double> dist(m_img.width(), m_img.height());
        for (int x = 0; x < dist.width(); x++)
        {
          for (int y = 0; y < dist.height(); y++)
            dist(x, y) = std::numeric_limits<double>::max();
        }

        // Iteration over all the clusters
        for (int cur = 0; cur < m_K; cur++)
        {
          const int x_begin = std::max(0, m_cx[cur] - m_spacing);
          const int x_end   = std::min(m_img.width() - 1, m_cx[cur] + m_spacing);
          const int y_begin = std::max(0, m_cy[cur] - m_spacing);
          const int y_end   = std::min(m_img.height() - 1, m_cy[cur] + m_spacing);

          for (int y = y_begin; y <= y_end; ++y)
          {
            for (int x = x_begin; x <= x_end; ++x)
            {
              const auto dxy  = std::sqrt((m_cx[cur] - x) * (m_cx[cur] - x) + (m_cy[cur] - y) * (m_cy[cur] - y));
              const auto drgb = std::sqrt((m_cr[cur] - m_img(x, y).red()) * (m_cr[cur] - m_img(x, y).red()) +
                                          (m_cg[cur] - m_img(x, y).green()) * (m_cg[cur] - m_img(x, y).green()) +
                                          (m_cb[cur] - m_img(x, y).blue()) * (m_cb[cur] - m_img(x, y).blue()));
              const auto d    = drgb + (static_cast<double>(m_compactness) / m_spacing) * dxy;

              if (d < dist(x, y))
              {
                dist(x, y)    = d;
                m_label(x, y) = cur;
              }
            }
          }
        }

        // Computing new cluster centers
        auto sum_x = std::make_unique<int[]>(m_K);
        auto sum_y = std::make_unique<int[]>(m_K);
        auto sum_r = std::make_unique<std::uint32_t[]>(m_K);
        auto sum_g = std::make_unique<std::uint32_t[]>(m_K);
        auto sum_b = std::make_unique<std::uint32_t[]>(m_K);
        auto area  = std::make_unique<int[]>(m_K);

        for (int y = 0; y < m_img.height(); ++y)
        {
          for (int x = 0; x < m_img.width(); ++x)
          {
            sum_x[m_label(x, y)] += x;
            sum_y[m_label(x, y)] += y;
            sum_r[m_label(x, y)] += m_img(x, y).red();
            sum_g[m_label(x, y)] += m_img(x, y).green();
            sum_b[m_label(x, y)] += m_img(x, y).blue();
            area[m_label(x, y)]++;
          }
        }

        for (int cur = 0; cur < m_K; ++cur)
        {
          m_cx[cur] = sum_x[cur] / area[cur];
          m_cy[cur] = sum_y[cur] / area[cur];
          m_cr[cur] = sum_r[cur] / area[cur];
          m_cg[cur] = sum_g[cur] / area[cur];
          m_cb[cur] = sum_b[cur] / area[cur];
        }
      }

    private:
      const image2d<colors::rgb8>     m_img;
      image2d<L>                      m_label;
      std::unique_ptr<int[]>          m_cx;
      std::unique_ptr<int[]>          m_cy;
      std::unique_ptr<std::uint8_t[]> m_cr;
      std::unique_ptr<std::uint8_t[]> m_cg;
      std::unique_ptr<std::uint8_t[]> m_cb;

      const int m_K;
      const int m_compactness;
      const int m_iter;
      const int m_spacing;
    };
  } // namespace impl

  template <typename L>
  image2d<L> slic(const image2d<colors::rgb8>& img, int K = 100, int compactness = 10, int iter = 10)
  {
    impl::SLICImplementation<L> algo(img, K, compactness, iter);
    algo.execute();
    return algo.label();
  }
} // namespace zagan::labeling