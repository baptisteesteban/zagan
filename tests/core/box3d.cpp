#include <zagan/core/domains/box3d.hpp>

#include <gtest/gtest.h>

using domain_t = zagan::box3d;
using point_t  = domain_t::point_t;

TEST(Core, Box3d_default)
{
  constexpr point_t  p;
  constexpr domain_t domain;
  ASSERT_EQ(domain.pmin(), p);
  ASSERT_EQ(domain.pmax(), p);
  ASSERT_EQ(domain.shape(0), 0);
  ASSERT_EQ(domain.shape(1), 0);
  ASSERT_EQ(domain.shape(2), 0);
}

TEST(Core, Box3d_size)
{
  constexpr domain_t domain(3, 5, 4);
  ASSERT_EQ(domain.pmin(), point_t());
  ASSERT_EQ(domain.pmax(), point_t(3, 5, 4));
  ASSERT_EQ(domain.shape(0), 3);
  ASSERT_EQ(domain.shape(1), 5);
  ASSERT_EQ(domain.shape(2), 4);
}

TEST(Core, Box3d_has)
{
  constexpr domain_t domain(3, 5, 4);
  ASSERT_TRUE(domain.has(point_t{}));
  ASSERT_TRUE(domain.has(point_t{2, 2, 2}));
  ASSERT_FALSE(domain.has(point_t{-1, 0, 0}));
  ASSERT_FALSE(domain.has(point_t{0, -1, 0}));
  ASSERT_FALSE(domain.has(point_t{0, 0, -1}));
  ASSERT_FALSE(domain.has(point_t{3, 5, 4}));
}
