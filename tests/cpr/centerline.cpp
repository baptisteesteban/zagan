#include <cmath>
#include <gtest/gtest.h>
#include <zagan/cpr/centerline.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace zagan;
using namespace zagan::cpr;

TEST(CPR, Centerline_ArcLength_Straight)
{
  std::vector<vec3> points = {vec3(0.0, 0.0, 0.0), vec3(10.0, 0.0, 0.0), vec3(20.0, 0.0, 0.0)};
  centerline        cl(points);

  ASSERT_DOUBLE_EQ(cl.arc_length(), 20.0);
}

TEST(CPR, Centerline_ArcLength_Circle)
{
  std::vector<vec3> points;
  double            radius = 10.0;
  int               steps  = 1000;
  for (int i = 0; i <= steps; ++i)
  {
    double theta = (i / static_cast<double>(steps)) * (M_PI / 2.0);
    points.push_back(vec3(radius * std::cos(theta), radius * std::sin(theta), 0.0));
  }

  centerline cl(points);
  double     expected_length = (M_PI / 2.0) * radius;

  ASSERT_NEAR(cl.arc_length(), expected_length, 1e-4);
}

TEST(CPR, Centerline_Tangent_Straight)
{
  std::vector<vec3> points;
  for (int i = 0; i < 10; ++i)
  {
    points.push_back(vec3(static_cast<double>(i), static_cast<double>(i), static_cast<double>(i)));
  }

  centerline cl(points);
  vec3       expected_dir = normalized(vec3(1.0, 1.0, 1.0));

  for (std::size_t i = 0; i < cl.size(); ++i)
  {
    vec3 t = cl.tangent(i);
    ASSERT_NEAR(t.x(), expected_dir.x(), 1e-9);
    ASSERT_NEAR(t.y(), expected_dir.y(), 1e-9);
    ASSERT_NEAR(t.z(), expected_dir.z(), 1e-9);
  }
}

TEST(CPR, Centerline_Resampled)
{
  std::vector<vec3> points = {vec3(0.0, 0.0, 0.0), vec3(10.0, 0.0, 0.0), vec3(10.0, 5.0, 0.0)};
  centerline        cl(points);

  double     step = 2.0;
  centerline r    = cl.resampled(step);

  ASSERT_GT(r.size(), cl.size());

  for (std::size_t i = 1; i < r.size() - 1; ++i)
  {
    double dist = norm(r.points()[i] - r.points()[i - 1]);
    ASSERT_NEAR(dist, step, 1e-9);
  }

  ASSERT_NEAR(r.arc_length(), cl.arc_length(), 1e-9);

  ASSERT_NEAR(r.points().back().x(), cl.points().back().x(), 1e-9);
  ASSERT_NEAR(r.points().back().y(), cl.points().back().y(), 1e-9);
  ASSERT_NEAR(r.points().back().z(), cl.points().back().z(), 1e-9);
}