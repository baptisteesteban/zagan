#include <zagan/core/points/point3d.hpp>

#include <gtest/gtest.h>

TEST(Core, Point3d_default)
{
  zagan::point3d p;
  ASSERT_EQ(p[0], 0);
  ASSERT_EQ(p[1], 0);
  ASSERT_EQ(p[2], 0);
  ASSERT_EQ(p.ndim(), 3);
  ASSERT_EQ(p, zagan::point3d(0, 0, 0));
}

TEST(Core, Point3d)
{
  zagan::point3d p{1, 8, 5};
  ASSERT_EQ(p[0], 1);
  ASSERT_EQ(p[1], 8);
  ASSERT_EQ(p[2], 5);
  ASSERT_EQ(p.x(), 1);
  ASSERT_EQ(p.y(), 8);
  ASSERT_EQ(p.z(), 5);
}
