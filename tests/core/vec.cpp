#include <zagan/core/vec.hpp>

#include <gtest/gtest.h>

#include <cstdint>

TEST(Vec, Constructors)
{
  using vec_t = zagan::vec<int, 3>;

  vec_t v1{1, 12, 7};
  vec_t v2(v1);
  ASSERT_EQ(v1, v2);
  vec_t v3 = v1;
  ASSERT_EQ(v1, v3);
  vec_t v4(std::move(v2));
  ASSERT_EQ(v1, v4);
  auto v5 = std::move(v3);
  ASSERT_EQ(v1, v5);
  vec_t v6;
  vec_t v7{0, 0, 0};
  ASSERT_EQ(v6, v7);
}

TEST(Vec, Operations)
{
  using namespace zagan;
  using vec_t  = zagan::vec<int, 3>;
  using vec8_t = zagan::vec<std::uint8_t, 3>;

  {
    vec_t v1{12, 43, 9};
    vec_t v2{52, 45, 36};
    vec_t res = v1 + v2;
    vec_t ref{64, 88, 45};
    ASSERT_EQ(res, ref);
  }

  // Test overflow +
  {
    vec8_t                       v1{255, 255, 255};
    vec8_t                       v2{1, 1, 1};
    auto                         res = v1 + v2;
    zagan::vec<std::uint16_t, 3> ref{256, 256, 256};
    ASSERT_EQ(res, ref);
  }

  {
    vec_t v1{12, 17, 31};
    vec_t v2{8, 19, 30};
    auto  res = v1 - v2;
    vec_t ref{4, -2, 1};
    ASSERT_EQ(res, ref);
  }

  // Test overflow -
  {
    vec8_t v1{0, 0, 1};
    vec8_t v2{1, 4, 0};
    auto   res = v1 - v2;
    vec_t  ref{-1, -4, 1};
    ASSERT_EQ(res, ref);
  }

  {
    vec_t v1{6, 8, 2};
    vec_t v2{2, 9, 12};
    auto  res = v1 * v2;
    vec_t ref{12, 72, 24};
    ASSERT_EQ(res, ref);

    const int lmd = 2;
    vec_t     ref2{12, 16, 4};
    ASSERT_EQ((lmd * v1), ref2);
    ASSERT_EQ((v1 * lmd), ref2);
  }
}

TEST(Vec, Format)
{
  using vec_t = zagan::vec<float, 3>;
  vec_t v{10, -38, 2.98};
  auto  res = std::format("{}", v);
  ASSERT_EQ(res, "(10, -38, 2.98)");
}