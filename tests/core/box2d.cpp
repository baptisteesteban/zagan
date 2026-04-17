#include <zagan/core/domains/box2d.hpp>

#include <gtest/gtest.h>

using domain_t = zagan::box2d;
using point_t  = domain_t::point_t;

TEST(Core, Box2d_default)
{
  constexpr point_t  p;
  constexpr domain_t domain;
  ASSERT_EQ(domain.pmin(), p);
  ASSERT_EQ(domain.pmax(), p);
  ASSERT_EQ(domain.shape(0), 0);
  ASSERT_EQ(domain.shape(1), 0);
}

TEST(Core, Box2d_size)
{
  constexpr domain_t domain(3, 5);
  ASSERT_EQ(domain.pmin(), point_t());
  ASSERT_EQ(domain.pmax(), point_t(3, 5));
  ASSERT_EQ(domain.shape(0), 3);
  ASSERT_EQ(domain.shape(1), 5);
}