#include <zagan/core/volume/volume.hpp>

#include <gtest/gtest.h>

TEST(Core, Volume_default)
{
  zagan::volume<float> vol;
  ASSERT_EQ(vol.width(), 0);
  ASSERT_EQ(vol.height(), 0);
  ASSERT_EQ(vol.depth(), 0);

  ASSERT_EQ(vol.spacing().x(), 1.0);
  ASSERT_EQ(vol.spacing().y(), 1.0);
  ASSERT_EQ(vol.spacing().z(), 1.0);

  ASSERT_EQ(vol.origin().x(), 0.0);
  ASSERT_EQ(vol.origin().y(), 0.0);
  ASSERT_EQ(vol.origin().z(), 0.0);
}

TEST(Core, Volume_init)
{
  zagan::vec3        spacing{2.0, 1.5, 3.0};
  zagan::vec3        origin{-10.0, -20.0, 5.0};
  zagan::volume<int> vol(10, 20, 30, spacing, origin);

  ASSERT_EQ(vol.width(), 10);
  ASSERT_EQ(vol.height(), 20);
  ASSERT_EQ(vol.depth(), 30);

  ASSERT_DOUBLE_EQ(vol.spacing().x(), 2.0);
  ASSERT_DOUBLE_EQ(vol.spacing().y(), 1.5);
  ASSERT_DOUBLE_EQ(vol.spacing().z(), 3.0);

  ASSERT_DOUBLE_EQ(vol.origin().x(), -10.0);
  ASSERT_DOUBLE_EQ(vol.origin().y(), -20.0);
  ASSERT_DOUBLE_EQ(vol.origin().z(), 5.0);
}

TEST(Core, Volume_to_world)
{
  zagan::vec3          spacing{2.0, 2.0, 2.0};
  zagan::vec3          origin{1.0, 1.0, 1.0};
  zagan::volume<float> vol(10, 10, 10, spacing, origin);

  zagan::point3d voxel{2, 3, 4};
  zagan::vec3    world = vol.to_world(voxel);

  // expected results :
  // world = origin + voxel * spacing
  // x: 1.0 + 2 * 2.0 = 5.0
  // y: 1.0 + 3 * 2.0 = 7.0
  // z: 1.0 + 4 * 2.0 = 9.0
  ASSERT_DOUBLE_EQ(world.x(), 5.0);
  ASSERT_DOUBLE_EQ(world.y(), 7.0);
  ASSERT_DOUBLE_EQ(world.z(), 9.0);
}

TEST(Core, Volume_to_voxel)
{
  zagan::vec3          spacing{0.5, 0.5, 0.5};
  zagan::vec3          origin{-5.0, 0.0, 5.0};
  zagan::volume<float> vol(20, 20, 20, spacing, origin);

  zagan::vec3 world{-2.0, 2.0, 7.0};
  zagan::vec3 voxel = vol.to_voxel(world);

  // expected results :
  // voxel = (world - origin) / spacing
  // x: (-2.0 - (-5.0)) / 0.5 = 3.0 / 0.5 = 6.0
  // y: (2.0 - 0.0) / 0.5 = 2.0 / 0.5 = 4.0
  // z: (7.0 - 5.0) / 0.5 = 2.0 / 0.5 = 4.0
  ASSERT_DOUBLE_EQ(voxel.x(), 6.0);
  ASSERT_DOUBLE_EQ(voxel.y(), 4.0);
  ASSERT_DOUBLE_EQ(voxel.z(), 4.0);
}
