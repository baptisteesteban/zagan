#include <zagan/image2d.hpp>

#include <gtest/gtest.h>

TEST(Core, Image2d_default_constructor)
{
  zagan::image2d<std::uint8_t> img;
  ASSERT_EQ(img.width(), 0);
  ASSERT_EQ(img.height(), 0);
}