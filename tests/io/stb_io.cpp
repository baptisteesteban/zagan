#include <zagan/io/imread.hpp>
#include <zagan/io/imsave.hpp>

#include <gtest/gtest.h>

TEST(IO, stb_uint8)
{
  std::uint8_t values[] = {1, 3, 7, 3, 0, 6};

  auto out = zagan::image2d<std::uint8_t>(3, 2);
  for (int i = 0; i < 6; i++)
    out(i % 3, i / 3) = values[i];

  zagan::io::imsave("tmp_uint8.png", out);
  zagan::image2d<std::uint8_t> in;
  zagan::io::imread("tmp_uint8.png", in);
  ASSERT_EQ(in.width(), out.width());
  ASSERT_EQ(in.height(), out.height());
  for (int i = 0; i < 6; i++)
    ASSERT_EQ(in(i % 3, i / 3), values[i]);
}