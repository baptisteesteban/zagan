#include <zagan/core/interpolation/bounds.hpp>
#include <zagan/core/interpolation/linear.hpp>
#include <zagan/core/interpolation/nearest.hpp>
#include <zagan/core/volume/volume.hpp>

#include <gtest/gtest.h>


TEST(Core, InterpolationBounds_Clamp)
{
  zagan::volume<float> vol(2, 2, 2);
  vol(0, 0, 0) = 1.0f;
  vol(1, 0, 0) = 2.0f;
  vol(0, 1, 0) = 3.0f;

  zagan::bounds::clamp clamp;
  ASSERT_EQ(clamp.get(vol, zagan::point3d(-1, 0, 0)), 1.0f);
  ASSERT_EQ(clamp.get(vol, zagan::point3d(2, 0, 0)), 2.0f);
  ASSERT_EQ(clamp.get(vol, zagan::point3d(0, 2, 0)), 3.0f);
}

TEST(Core, InterpolationBounds_Padding)
{
  zagan::volume<float> vol(2, 2, 2);
  vol(1, 0, 0) = 2.0f;

  zagan::bounds::padding<float> pad(42.0f);
  ASSERT_EQ(pad.get(vol, zagan::point3d(-1, 0, 0)), 42.0f);
  ASSERT_EQ(pad.get(vol, zagan::point3d(1, 0, 0)), 2.0f);
}

TEST(Core, LinearInterpolator)
{
  zagan::vec3          spacing{1.0, 1.0, 1.0};
  zagan::vec3          origin{0.0, 0.0, 0.0};
  zagan::volume<float> vol(2, 2, 2, spacing, origin);

  for (int z = 0; z < 2; z++)
  {
    for (int y = 0; y < 2; y++)
    {
      for (int x = 0; x < 2; x++)
      {
        vol(x, y, z) = static_cast<float>(x + y * 2 + z * 4);
      }
    }
  }

  zagan::linear_interpolator<> interp;
  ASSERT_FLOAT_EQ(interp(vol, zagan::vec3(0.0, 0.0, 0.0)), 0.0f);
  ASSERT_FLOAT_EQ(interp(vol, zagan::vec3(0.5, 0.0, 0.0)), 0.5f);
  ASSERT_FLOAT_EQ(interp(vol, zagan::vec3(0.5, 0.5, 0.0)), 1.5f);
  ASSERT_FLOAT_EQ(interp(vol, zagan::vec3(0.5, 0.5, 0.5)), 3.5f);
  ASSERT_FLOAT_EQ(interp(vol, zagan::vec3(1.0, 1.0, 1.0)), 7.0f);
}

TEST(Core, NearestInterpolator)
{
  zagan::vec3          spacing{1.0, 1.0, 1.0};
  zagan::vec3          origin{0.0, 0.0, 0.0};
  zagan::volume<float> vol(2, 2, 2, spacing, origin);

  for (int z = 0; z < 2; z++)
  {
    for (int y = 0; y < 2; y++)
    {
      for (int x = 0; x < 2; x++)
      {
        vol(x, y, z) = static_cast<float>(x + y * 2 + z * 4);
      }
    }
  }

  zagan::nearest_interpolator<> interp;
  ASSERT_FLOAT_EQ(interp(vol, zagan::vec3(0.1, 0.1, 0.1)), 0.0f);
  ASSERT_FLOAT_EQ(interp(vol, zagan::vec3(0.9, 0.1, 0.1)), 1.0f);
}