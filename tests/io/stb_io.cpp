#include <stdexcept>
#include <zagan/io/imread.hpp>
#include <zagan/io/imsave.hpp>

#include <gtest/gtest.h>

TEST(IO, stb_uint8)
{
  std::uint8_t values[] = {1, 3, 7, 3, 0, 6};

  auto out = zagan::image2d<std::uint8_t>{{1, 3, 7}, {3, 0, 6}};

  zagan::io::imsave("tmp_uint8.png", out);
  zagan::image2d<std::uint8_t> in;
  zagan::io::imread("tmp_uint8.png", in);
  ASSERT_EQ(in.width(), out.width());
  ASSERT_EQ(in.height(), out.height());
  for (int y = 0; y < out.height(); y++)
  {
    for (int x = 0; x < out.width(); x++)
      ASSERT_EQ(in(x, y), out(x, y));
  }

  try
  {
    zagan::io::imsave("oops.jpg", out);
    ASSERT_TRUE(false);
  }
  catch (const std::invalid_argument&)
  {
  }
}