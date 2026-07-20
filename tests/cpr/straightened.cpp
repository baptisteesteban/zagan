#include <gtest/gtest.h>

#include <zagan/core/interpolation/bounds.hpp>
#include <zagan/core/interpolation/linear.hpp>
#include <zagan/cpr/straightened.hpp>
#include <zagan/io/imsave.hpp>

#include "scan_fixture.hpp"

#include <cstdint>
#include <format>
#include <fstream>
#include <string>

using namespace zagan;
using namespace zagan::cpr;
using namespace zagan::testing;

struct constant_interpolator
{
  float value;
  float operator()(const volume<float>&, const vec3&) const noexcept { return value; }
};

TEST(CPR, StraightenedMpr_OutputDimensions)
{
  std::vector<vec3> pts;
  for (int i = 0; i <= 10; ++i)
    pts.push_back(vec3(static_cast<double>(i), 0.0, 0.0));
  centerline cl(pts);

  straightened_params p{10.0, 50, 8};
  volume<float>       vol(20, 20, 20, vec3(1.0, 1.0, 1.0));

  image3d<float> out = straighten_mpr(vol, cl, p, constant_interpolator{0.0f});

  centerline resampled = cl.resampled(p.width_mm / p.columns);
  ASSERT_EQ(out.width(), p.columns);
  ASSERT_EQ(out.height(), static_cast<int>(resampled.size()));
  ASSERT_EQ(out.depth(), p.num_angles);
}

TEST(CPR, StraightenedMpr_ConstantVolume)
{
  std::vector<vec3> pts;
  for (int i = 0; i <= 20; ++i)
    pts.push_back(vec3(static_cast<double>(i), 0.0, 0.0));
  centerline cl(pts);

  straightened_params p{10.0, 20, 8};
  image3d<float>      out =
      straighten_mpr(volume<float>(30, 30, 30, vec3(1.0, 1.0, 1.0)), cl, p, constant_interpolator{42.0f});

  for (int z = 0; z < out.depth(); ++z)
    for (int y = 0; y < out.height(); ++y)
      for (int x = 0; x < out.width(); ++x)
        ASSERT_FLOAT_EQ(out(x, y, z), 42.0f);
}

TEST(CPR, StraightenedMpr_Isometry)
{
  std::vector<vec3> pts;
  for (int i = 0; i <= 50; ++i)
    pts.push_back(vec3(static_cast<double>(i), 0.0, 0.0));
  centerline cl(pts);

  straightened_params p{10.0, 20, 8};
  double              step_mm = p.width_mm / p.columns;
  volume<float>       vol(60, 10, 10, vec3(1.0, 1.0, 1.0));

  image3d<float> out = straighten_mpr(vol, cl, p, constant_interpolator{0.0f});

  ASSERT_NEAR(static_cast<double>(out.height()) * step_mm, cl.arc_length(), step_mm);
}

static void render_case(int case_id)
{
  const std::string nii_path = scan_path(case_id);
  const std::string csv_path = centerline_path(case_id);

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

  volume<int16_t> scan_volume       = read_scan_volume(nii_path);
  centerline      vessel_centerline = load_centerline_csv(csv_path);
  ASSERT_GT(vessel_centerline.size(), 1u);

  const straightened_params params{40.0, 200, 20};

  image3d<int16_t> stack = straighten_mpr(scan_volume, vessel_centerline, params, linear_interpolator<bounds::clamp>{});

  ASSERT_GT(stack.width(), 0);
  ASSERT_GT(stack.height(), 0);
  ASSERT_EQ(stack.depth(), params.num_angles);

  for (int angle = 0; angle < stack.depth(); ++angle)
  {
    const std::string output_path = std::format("smpr_case{:03d}_angle{:02d}.png", case_id, angle);
    io::imsave(output_path.c_str(), window_slice_to_gray(stack, angle));
  }
}

TEST(CPR_Real, StraightenedMpr_Render)
{
  const std::vector<int> cases = available_cases();
  if (cases.empty())
    GTEST_SKIP() << "No cases installed under " << k_img_dir;
  for (int id : cases)
    render_case(id);
}
