#include <gtest/gtest.h>
#include <zagan/cpr/straightened.hpp>

using namespace zagan;
using namespace zagan::cpr;

struct constant_interpolator
{
  float value;
  float operator()(const volume<float>&, const vec3&) const noexcept { return value; }
};

TEST(CPR, Straightened_OutputDimensions)
{
  std::vector<vec3> pts;
  for (int i = 0; i <= 10; ++i)
    pts.push_back(vec3(static_cast<double>(i), 0.0, 0.0));
  centerline cl(pts);

  straightened_params p{10.0, 50};
  volume<float>       vol(20, 20, 20, vec3(1.0, 1.0, 1.0));

  image2d<float> out = straighten(vol, cl, p, constant_interpolator{0.0f});

  centerline resampled = cl.resampled(p.width_mm / p.columns);
  ASSERT_EQ(out.width(), p.columns);
  ASSERT_EQ(out.height(), static_cast<int>(resampled.size()));
}

TEST(CPR, Straightened_ConstantVolume)
{
  std::vector<vec3> pts;
  for (int i = 0; i <= 20; ++i)
    pts.push_back(vec3(static_cast<double>(i), 0.0, 0.0));
  centerline cl(pts);

  straightened_params p{10.0, 20};
  image2d<float> out = straighten(volume<float>(30, 30, 30, vec3(1.0, 1.0, 1.0)), cl, p, constant_interpolator{42.0f});

  for (int y = 0; y < out.height(); ++y)
    for (int x = 0; x < out.width(); ++x)
      ASSERT_FLOAT_EQ(out(x, y), 42.0f);
}

TEST(CPR, Straightened_Isometry)
{
  std::vector<vec3> pts;
  for (int i = 0; i <= 50; ++i)
    pts.push_back(vec3(static_cast<double>(i), 0.0, 0.0));
  centerline cl(pts);

  straightened_params p{10.0, 20};
  double              step_mm = p.width_mm / p.columns;
  volume<float>       vol(60, 10, 10, vec3(1.0, 1.0, 1.0));

  image2d<float> out = straighten(vol, cl, p, constant_interpolator{0.0f});

  ASSERT_NEAR(static_cast<double>(out.height()) * step_mm, cl.arc_length(), step_mm);
}
