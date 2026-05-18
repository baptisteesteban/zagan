#include <zagan/io/nifti_reader.hpp>

#include <gtest/gtest.h>

#include <cstring>
#include <fstream>

static const std::string FIXTURE_PATH   = ZAGAN_TEST_DATA_DIR "/tiny.nii";
static const std::string REAL_SCAN_PATH = ZAGAN_TEST_DATA_DIR "/1.img.nii";

TEST(IO, NiftiReader_header_size)
{
  std::ifstream file(FIXTURE_PATH, std::ios::binary);
  auto          hdr = zagan::io::detail::read_header(file);
  ASSERT_EQ(hdr.sizeof_hdr, 348);
}

TEST(IO, NiftiReader_header_truncated_file_throws)
{
  std::ifstream file(FIXTURE_PATH, std::ios::binary);
  file.seekg(300);
  ASSERT_THROW(zagan::io::detail::read_header(file), std::runtime_error);
}

TEST(IO, NiftiReader_valid_header)
{
  std::ifstream file(FIXTURE_PATH, std::ios::binary);
  auto          hdr = zagan::io::detail::read_header(file);
  ASSERT_NO_THROW(zagan::io::detail::validate_header(hdr));
}

TEST(IO, NiftiReader_invalid_magic_throws)
{
  auto hdr = zagan::io::detail::nifti1_header{};
  std::memcpy(hdr.magic, "bad\0", 4);
  ASSERT_THROW(zagan::io::detail::validate_header(hdr), std::runtime_error);
}

TEST(IO, NiftiReader_invalid_dimensions_throws)
{
  auto hdr = zagan::io::detail::nifti1_header{};
  std::memcpy(hdr.magic, "n+1\0", 4);
  hdr.vox_offset = 352.0f;
  hdr.dim[0]     = 4;
  ASSERT_THROW(zagan::io::detail::validate_header(hdr), std::runtime_error);
}

TEST(IO, NiftiReader_unsupported_datatype_throws)
{
  auto hdr = zagan::io::detail::nifti1_header{};
  std::memcpy(hdr.magic, "n+1\0", 4);
  hdr.vox_offset = 352.0f;
  hdr.dim[0]     = 3;
  hdr.dim[1] = hdr.dim[2] = hdr.dim[3] = 1;
  hdr.datatype                         = 9999;
  ASSERT_THROW(zagan::io::detail::validate_header(hdr), std::runtime_error);
}

TEST(IO, NiftiReader_dimensions)
{
  auto img = zagan::io::read_nifti<int16_t>(FIXTURE_PATH);
  ASSERT_EQ(img.width(), 4);
  ASSERT_EQ(img.height(), 5);
  ASSERT_EQ(img.depth(), 6);
}

TEST(IO, NiftiReader_voxel_values)
{
  auto img = zagan::io::read_nifti<int16_t>(FIXTURE_PATH);
  ASSERT_EQ(img(0, 0, 0), 42);
  ASSERT_EQ(img(1, 0, 0), 0);
}

TEST(IO, NiftiReader_type_mismatch_throws)
{
  ASSERT_THROW(zagan::io::read_nifti<float>(FIXTURE_PATH), std::runtime_error);
}

TEST(IO, NiftiReader_missing_file_throws)
{
  ASSERT_THROW(zagan::io::read_nifti<int16_t>("does_not_exist.nii"), std::runtime_error);
}

TEST(IO, NiftiReader_real_scan)
{
  if (!std::ifstream(REAL_SCAN_PATH))
    GTEST_SKIP() << "real scan not present, skipping";
  auto img = zagan::io::read_nifti<int16_t>(REAL_SCAN_PATH);
  ASSERT_GT(img.width(), 0);
  ASSERT_GT(img.height(), 0);
  ASSERT_GT(img.depth(), 0);
}
