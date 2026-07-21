#include <gtest/gtest.h>

#include <zagan/core/volume/volume.hpp>
#include <zagan/cpr/cmpr.hpp>
#include <zagan/io/imsave.hpp>

#include "scan_fixture.hpp"

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

using namespace zagan;
using namespace zagan::cpr;
using namespace zagan::testing;

static void run_case(int case_id)
{
  const std::string nii_path    = scan_path(case_id);
  const std::string csv_path    = centerline_path(case_id);
  const std::string output_path = "cmpr_case" + std::to_string(case_id) + ".png";

  if (!std::ifstream(nii_path))
  {
    GTEST_SKIP() << "NIfTI not found: " << nii_path;
    return;
  }
  if (!std::ifstream(csv_path))
  {
    GTEST_SKIP() << "Centerline not found: " << csv_path;
    return;
  }

  volume<int16_t> scan_volume = read_scan_volume(nii_path);

  centerline vessel_centerline = load_centerline_csv(csv_path);
  ASSERT_GT(vessel_centerline.size(), 1u);

  const double           voxel_spacing_mm = std::abs(scan_volume.spacing().x());
  const image2d<int16_t> cmpr_image =
      stretched(vessel_centerline, scan_volume, vec3(0.0, 0.0, 1.0), 50, voxel_spacing_mm);

  ASSERT_GT(cmpr_image.width(), 0);
  ASSERT_GT(cmpr_image.height(), 0);

  io::imsave(output_path.c_str(), window_to_gray(cmpr_image));
}

static void run_case_20rotation(int case_id)
{
  const std::string nii_path = k_img_dir + "/" + std::to_string(case_id) + ".img.nii";
  const std::string csv_path = k_cl_dir + "/" + std::to_string(case_id) + ".csv";

  if (!std::ifstream(nii_path))
  {
    std::cerr << "NIfTI not found: " << nii_path << '\n';
    return;
  }

  if (!std::ifstream(csv_path))
  {
    std::cerr << "Centerline not found: " << csv_path << '\n';
    return;
  }

  volume<int16_t> scan_volume       = read_scan_volume(nii_path);
  centerline      vessel_centerline = load_centerline_csv(csv_path);

  if (vessel_centerline.size() < 2)
  {
    std::cerr << "Invalid centerline.\n";
    return;
  }

  const double voxel_spacing_mm = std::abs(scan_volume.spacing().x());

  constexpr int    nb_angles = 20;
  constexpr double pi        = 3.14159265358979323846;
  for (int i = 0; i < nb_angles; ++i)
  {
    const double angle = 2.0 * pi * static_cast<double>(i) / nb_angles;

    image2d<int16_t> cmpr =
        stretched(vessel_centerline, scan_volume, {std::cos(angle), std::sin(angle), 0.0}, 50, voxel_spacing_mm);
    image2d<uint8_t> output(cmpr.width(), cmpr.height());

    for (int y = 0; y < cmpr.height(); ++y)
      for (int x = 0; x < cmpr.width(); ++x)
        output(x, y) = apply_window_level(cmpr(x, y), -200.0f, 800.0f);

    const std::string output_path = std::format("{:02d}_id_projection_{:02d}.png", case_id, i);

    io::imsave(output_path.c_str(), output);
  }
}

TEST(CMPR_Real, Case01)
{
  const std::vector<int> cases = available_cases();
  if (cases.empty())
    GTEST_SKIP() << "No cases installed under " << k_img_dir;
  for (int id : cases)
    run_case(id);
}
