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
  using vec_t = zagan::vec<int, 3>;

  {
    vec_t v1{12, 43, 9};
    vec_t v2{52, 45, 36};
    vec_t res = v1 + v2;
    vec_t ref{64, 88, 45};
    ASSERT_EQ(res, ref);
  }

  // Test overflow +
  using vec8_t = zagan::vec<std::uint8_t, 3>;
  {
    vec8_t                       v1{255, 255, 255};
    vec8_t                       v2{1, 1, 1};
    auto                         res = v1 + v2;
    zagan::vec<std::uint16_t, 3> ref{256, 256, 256};
    ASSERT_EQ(res, ref);
  }
}