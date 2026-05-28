#include <cmath>
#include <gtest/gtest.h>
#include <zagan/cpr/frame.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace zagan;
using namespace zagan::cpr;

TEST(CPR, Frame_Empty)
{
  ASSERT_TRUE(rotation_minimizing_frame(centerline{}).empty());

  centerline single({vec3(0.0, 0.0, 0.0)});
  ASSERT_TRUE(rotation_minimizing_frame(single).empty());
}

TEST(CPR, Frame_OnePerPoint)
{
  std::vector<vec3> points = {vec3(0.0, 0.0, 0.0), vec3(10.0, 0.0, 0.0), vec3(10.0, 5.0, 0.0)};
  centerline        cl(points);

  std::vector<frame> frames = rotation_minimizing_frame(cl);

  ASSERT_EQ(frames.size(), cl.size());
}

TEST(CPR, Frame_Orthonormal)
{
  std::vector<vec3> points;
  double            radius = 10.0;
  int               steps  = 200;
  for (int i = 0; i <= steps; ++i)
  {
    double theta = (i / static_cast<double>(steps)) * (M_PI / 2.0);
    points.push_back(vec3(radius * std::cos(theta), radius * std::sin(theta), 0.0));
  }

  centerline         cl(points);
  std::vector<frame> frames = rotation_minimizing_frame(cl);

  for (const frame& f : frames)
  {
    ASSERT_NEAR(norm(f.t), 1.0, 1e-9);
    ASSERT_NEAR(norm(f.u), 1.0, 1e-9);
    ASSERT_NEAR(norm(f.v), 1.0, 1e-9);

    ASSERT_NEAR(dot(f.t, f.u), 0.0, 1e-9);
    ASSERT_NEAR(dot(f.t, f.v), 0.0, 1e-9);
    ASSERT_NEAR(dot(f.u, f.v), 0.0, 1e-9);
  }
}

TEST(CPR, Frame_TangentMatchesCenterline)
{
  std::vector<vec3> points = {vec3(0.0, 0.0, 0.0), vec3(10.0, 0.0, 0.0), vec3(10.0, 5.0, 0.0), vec3(15.0, 5.0, 0.0)};
  centerline        cl(points);

  std::vector<frame> frames = rotation_minimizing_frame(cl);

  for (std::size_t i = 0; i < cl.size(); ++i)
  {
    vec3 t = cl.tangent(i);
    ASSERT_NEAR(frames[i].t.x(), t.x(), 1e-9);
    ASSERT_NEAR(frames[i].t.y(), t.y(), 1e-9);
    ASSERT_NEAR(frames[i].t.z(), t.z(), 1e-9);
  }
}

TEST(CPR, Frame_StraightLineNoRotation)
{
  std::vector<vec3> points;
  for (int i = 0; i < 10; ++i)
    points.push_back(vec3(static_cast<double>(i), 0.0, 0.0));

  centerline         cl(points);
  std::vector<frame> frames = rotation_minimizing_frame(cl);

  vec3 u0 = frames.front().u;
  vec3 v0 = frames.front().v;
  for (const frame& f : frames)
  {
    ASSERT_NEAR(f.u.x(), u0.x(), 1e-9);
    ASSERT_NEAR(f.u.y(), u0.y(), 1e-9);
    ASSERT_NEAR(f.u.z(), u0.z(), 1e-9);

    ASSERT_NEAR(f.v.x(), v0.x(), 1e-9);
    ASSERT_NEAR(f.v.y(), v0.y(), 1e-9);
    ASSERT_NEAR(f.v.z(), v0.z(), 1e-9);
  }
}

TEST(CPR, Frame_TangentAlongYAxis)
{
  std::vector<vec3> points;
  for (int i = 0; i < 5; ++i)
    points.push_back(vec3(0.0, static_cast<double>(i), 0.0));

  centerline         cl(points);
  std::vector<frame> frames = rotation_minimizing_frame(cl);

  ASSERT_EQ(frames.size(), cl.size());
  for (const frame& f : frames)
  {
    ASSERT_NEAR(norm(f.u), 1.0, 1e-9);
    ASSERT_NEAR(dot(f.t, f.u), 0.0, 1e-9);
  }
}
