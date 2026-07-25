#include <zagan/io/nifti_io.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <fstream>

static const std::string FIXTURE_PATH   = ZAGAN_TEST_DATA_DIR "/tiny.nii";
static const std::string REAL_SCAN_PATH = ZAGAN_TEST_DATA_DIR "/1.img.nii";

TEST(IO, NiftiIO_dimensions)
{
  auto vol = zagan::io::read_nifti<int16_t>(FIXTURE_PATH);
  ASSERT_EQ(vol.image.width(), 4);
  ASSERT_EQ(vol.image.height(), 5);
  ASSERT_EQ(vol.image.depth(), 6);
}

TEST(IO, NiftiIO_voxel_values)
{
  auto vol = zagan::io::read_nifti<int16_t>(FIXTURE_PATH);
  ASSERT_EQ(vol.image(0, 0, 0), 42);
  ASSERT_EQ(vol.image(1, 0, 0), 0);
}

TEST(IO, NiftiIO_missing_file_throws)
{
  ASSERT_THROW(zagan::io::read_nifti<int16_t>("does_not_exist.nii"), std::runtime_error);
}

TEST(IO, NiftiIO_roundtrip)
{
  auto              vol = zagan::io::read_nifti<int16_t>(FIXTURE_PATH);
  const std::string tmp = "roundtrip_test.nii";
  zagan::io::write_nifti(tmp, vol);

  auto vol2 = zagan::io::read_nifti<int16_t>(tmp);
  ASSERT_EQ(vol2.image.width(), vol.image.width());
  ASSERT_EQ(vol2.image.height(), vol.image.height());
  ASSERT_EQ(vol2.image.depth(), vol.image.depth());
  ASSERT_EQ(vol2.image(0, 0, 0), vol.image(0, 0, 0));
  ASSERT_EQ(vol2.image(1, 0, 0), vol.image(1, 0, 0));
}

TEST(IO, NiftiIO_real_scan)
{
  if (!std::ifstream(REAL_SCAN_PATH))
    GTEST_SKIP() << "real scan not present, skipping";
  auto vol = zagan::io::read_nifti<int16_t>(REAL_SCAN_PATH);
  ASSERT_GT(vol.image.width(), 0);
  ASSERT_GT(vol.image.height(), 0);
  ASSERT_GT(vol.image.depth(), 0);
}
