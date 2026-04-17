#include <zagan/core/image/image2d.hpp>

#include <gtest/gtest.h>

using image_t = zagan::image2d<std::uint8_t>;

TEST(Core, Image2d_default_constructor)
{
  image_t img;
  ASSERT_EQ(img.width(), 0);
  ASSERT_EQ(img.height(), 0);
  ASSERT_EQ(img.buffer(), nullptr);
}

TEST(Core, Image2d_basic_constructor)
{
  image_t img(7, 3);
  ASSERT_EQ(img.width(), 7);
  ASSERT_EQ(img.height(), 3);
  ASSERT_NE(img.buffer(), nullptr);
}

TEST(Core, Image2d_copy)
{
  image_t img(7, 3);

  image_t img2(img);
  ASSERT_EQ(img2.width(), img.width());
  ASSERT_EQ(img2.height(), img.height());
  ASSERT_EQ(img2.buffer(), img.buffer());

  auto img3 = img;
  ASSERT_EQ(img3.width(), img.width());
  ASSERT_EQ(img3.height(), img.height());
  ASSERT_EQ(img3.buffer(), img.buffer());
}

TEST(Core, Image2d_domain)
{
  image_t    img(7, 3);
  const auto domain = img.domain();
  ASSERT_EQ(domain.shape(0), 7);
  ASSERT_EQ(domain.shape(1), 3);
}