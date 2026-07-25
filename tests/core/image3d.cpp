#include <zagan/core/concepts/image_traits.hpp>
#include <zagan/core/image/image3d.hpp>

#include <gtest/gtest.h>

#include <concepts>

using image_t = zagan::image3d<std::uint8_t>;

TEST(Core, Image3d_default_constructor)
{
  image_t img;
  ASSERT_EQ(img.width(), 0);
  ASSERT_EQ(img.height(), 0);
  ASSERT_EQ(img.depth(), 0);
  ASSERT_EQ(img.buffer(), nullptr);
}

TEST(Core, Image3d_basic_constructor)
{
  image_t img(7, 3, 4);
  ASSERT_EQ(img.width(), 7);
  ASSERT_EQ(img.height(), 3);
  ASSERT_EQ(img.depth(), 4);
  ASSERT_NE(img.buffer(), nullptr);
}

TEST(Core, Image3d_copy)
{
  image_t img(7, 3, 4);

  image_t img2(img);
  ASSERT_EQ(img2.width(), img.width());
  ASSERT_EQ(img2.height(), img.height());
  ASSERT_EQ(img2.depth(), img.depth());
  ASSERT_EQ(img2.buffer(), img.buffer());

  auto img3 = img;
  ASSERT_EQ(img3.width(), img.width());
  ASSERT_EQ(img3.height(), img.height());
  ASSERT_EQ(img3.depth(), img.depth());
  ASSERT_EQ(img3.buffer(), img.buffer());
}

TEST(Core, Image3d_domain)
{
  image_t    img(7, 3, 4);
  const auto domain = img.domain();
  ASSERT_EQ(domain.shape(0), 7);
  ASSERT_EQ(domain.shape(1), 3);
  ASSERT_EQ(domain.shape(2), 4);
}

TEST(Core, Image3d_concept)
{
  static_assert(std::same_as<zagan::image_value_t<image_t>, std::uint8_t>);
  static_assert(std::same_as<zagan::image_domain_t<image_t>, zagan::box3d>);
  static_assert(std::same_as<zagan::image_point_t<image_t>, zagan::point3d>);
}
