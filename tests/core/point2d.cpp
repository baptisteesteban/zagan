#include <zagan/core/points/point2d.hpp>

#include <gtest/gtest.h>

TEST(Core, Point2d_default)
{
  zagan::point2d p;
  ASSERT_EQ(p[0], 0);
  ASSERT_EQ(p[1], 0);
  ASSERT_EQ(p.ndim(), 2);
}

TEST(Core, Point2d)
{
  zagan::point2d p{1, 8};
  ASSERT_EQ(p[0], 1);
  ASSERT_EQ(p[1], 8);
  ASSERT_EQ(p.x(), 1);
  ASSERT_EQ(p.y(), 8);
}