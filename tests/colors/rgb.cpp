#include <zagan/colors/rgb.hpp>

#include <gtest/gtest.h>

TEST(Colors, rgb8)
{
  zagan::colors::rgb8 c(12, 7, 9);
  ASSERT_EQ(c.red(), 12);
  ASSERT_EQ(c.green(), 7);
  ASSERT_EQ(c.blue(), 9);

  const zagan::colors::rgb8 cc({15, 9, 7});
  ASSERT_EQ(cc.red(), 15);
  ASSERT_EQ(cc.green(), 9);
  ASSERT_EQ(cc.blue(), 7);
}