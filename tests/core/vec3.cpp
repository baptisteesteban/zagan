#include <zagan/core/points/vec3.hpp>

#include <cmath>
#include <gtest/gtest.h>

TEST(Core, Vec3_default)
{
  zagan::vec3 v;
  ASSERT_DOUBLE_EQ(v.x(), 0.0);
  ASSERT_DOUBLE_EQ(v.y(), 0.0);
  ASSERT_DOUBLE_EQ(v.z(), 0.0);
  ASSERT_EQ(v.ndim(), 3);
}

TEST(Core, Vec3_init)
{
  zagan::vec3 v{1.5, -2.0, 3.1};
  ASSERT_DOUBLE_EQ(v.x(), 1.5);
  ASSERT_DOUBLE_EQ(v.y(), -2.0);
  ASSERT_DOUBLE_EQ(v.z(), 3.1);
}

TEST(Core, Vec3_addition)
{
  zagan::vec3 v1{1.0, 2.0, 3.0};
  zagan::vec3 v2{4.0, 5.0, 6.0};
  zagan::vec3 result = v1 + v2;
  ASSERT_DOUBLE_EQ(result.x(), 5.0);
  ASSERT_DOUBLE_EQ(result.y(), 7.0);
  ASSERT_DOUBLE_EQ(result.z(), 9.0);
}

TEST(Core, Vec3_subtraction)
{
  zagan::vec3 v1{1.0, 2.0, 3.0};
  zagan::vec3 v2{4.0, 5.0, 6.0};
  zagan::vec3 result = v1 - v2;
  ASSERT_DOUBLE_EQ(result.x(), -3.0);
  ASSERT_DOUBLE_EQ(result.y(), -3.0);
  ASSERT_DOUBLE_EQ(result.z(), -3.0);
}

TEST(Core, Vec3_scalar_multiplication)
{
  zagan::vec3 v1{1.0, -2.0, 3.0};
  zagan::vec3 r1 = v1 * 2.0;
  zagan::vec3 r2 = 3.0 * v1;

  ASSERT_DOUBLE_EQ(r1.x(), 2.0);
  ASSERT_DOUBLE_EQ(r1.y(), -4.0);
  ASSERT_DOUBLE_EQ(r1.z(), 6.0);

  ASSERT_DOUBLE_EQ(r2.x(), 3.0);
  ASSERT_DOUBLE_EQ(r2.y(), -6.0);
  ASSERT_DOUBLE_EQ(r2.z(), 9.0);
}

TEST(Core, Vec3_dot_product)
{
  zagan::vec3 v1{1.0, 2.0, 3.0};
  zagan::vec3 v2{4.0, -5.0, 6.0};
  double      result = zagan::dot(v1, v2);
  ASSERT_DOUBLE_EQ(result, 12.0); // 4 - 10 + 18
}

TEST(Core, Vec3_cross_product)
{
  zagan::vec3 v1{1.0, 0.0, 0.0};
  zagan::vec3 v2{0.0, 1.0, 0.0};
  zagan::vec3 result = zagan::cross(v1, v2);
  ASSERT_DOUBLE_EQ(result.x(), 0.0);
  ASSERT_DOUBLE_EQ(result.y(), 0.0);
  ASSERT_DOUBLE_EQ(result.z(), 1.0);
}

TEST(Core, Vec3_norm)
{
  zagan::vec3 v{3.0, 4.0, 0.0};
  ASSERT_DOUBLE_EQ(zagan::norm(v), 5.0);
}

TEST(Core, Vec3_normalized)
{
  zagan::vec3 v{0.0, 3.0, 4.0};
  zagan::vec3 n = zagan::normalized(v);
  ASSERT_DOUBLE_EQ(n.x(), 0.0);
  ASSERT_DOUBLE_EQ(n.y(), 3.0 / 5.0);
  ASSERT_DOUBLE_EQ(n.z(), 4.0 / 5.0);
  ASSERT_DOUBLE_EQ(zagan::norm(n), 1.0);
}
