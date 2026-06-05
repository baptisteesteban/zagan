#include <zagan/colors/rgb.hpp>
#include <zagan/io/imread.hpp>
#include <zagan/io/imsave.hpp>

#include <stdexcept>

#include <gtest/gtest.h>

TEST(IO, stb_uint8)
{
  zagan::image2d<std::uint8_t> out{{1, 3, 7}, {3, 0, 6}};

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

TEST(IO, stb_rgb88)
{
  zagan::image2d<zagan::colors::rgb8> out{{{2, 5, 7}, {3, 2, 9}, {12, 99, 17}},
                                          {{75, 13, 43}, {39, 75, 47}, {28, 43, 85}}};

  zagan::io::imsave("tmp_rgb8.png", out);
  zagan::image2d<zagan::colors::rgb8> in;
  zagan::io::imread("tmp_rgb8.png", in);
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