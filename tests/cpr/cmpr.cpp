#include <gtest/gtest.h>

#include <zagan/core/volume/volume.hpp>
#include <zagan/cpr/cmpr.hpp>
#include <zagan/io/imsave.hpp>
#include <zagan/io/nifti_reader.hpp>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace zagan;
using namespace zagan::cpr;

// srow_x/y/z in header
static volume<int16_t> read_scan_volume(const std::string& path)
{
  float srow_x[4], srow_y[4], srow_z[4];
  {
    std::ifstream f(path, std::ios::binary);
    f.seekg(280);
    f.read(reinterpret_cast<char*>(srow_x), 16);
    f.seekg(296);
    f.read(reinterpret_cast<char*>(srow_y), 16);
    f.seekg(312);
    f.read(reinterpret_cast<char*>(srow_z), 16);
  }

  const vec3       spacing(srow_x[0], srow_y[1], srow_z[2]);
  const vec3       origin(srow_x[3], srow_y[3], srow_z[3]);
  image3d<int16_t> img = io::read_nifti<int16_t>(path);

  volume<int16_t>   vol(img.width(), img.height(), img.depth(), spacing, origin);
  const std::size_t n = static_cast<std::size_t>(img.width()) * img.height() * img.depth();
  std::copy(img.buffer(), img.buffer() + n, vol.buffer());
  return vol;
}

static const std::filesystem::path k_project_root =
    std::filesystem::path(__FILE__).parent_path().parent_path().parent_path();
static const std::string k_img_dir = (k_project_root / "data/3dimg/extracted").string();
static const std::string k_cl_dir  = (k_project_root / "data/pfee/centerlines/centerlines").string();

static centerline load_centerline_csv(const std::string& csv_path)
{
  std::ifstream csv_file(csv_path);
  std::string   line;
  std::getline(csv_file, line); // skip header
  std::vector<vec3>   points;
  std::vector<double> radii;
  while (std::getline(csv_file, line))
  {
    if (line.empty())
      continue;
    std::istringstream line_stream(line);
    std::string        token;
    std::getline(line_stream, token, ','); // index
    std::getline(line_stream, token, ','); // s_mm
    std::getline(line_stream, token, ',');
    double x = std::stod(token);
    std::getline(line_stream, token, ',');
    double y = std::stod(token);
    std::getline(line_stream, token, ',');
    double z = std::stod(token);
    std::getline(line_stream, token, ',');
    double radius = std::stod(token);
    points.push_back(vec3(x, y, z));
    radii.push_back(radius);
  }
  return centerline(std::move(points), std::move(radii));
}

static uint8_t apply_window_level(int16_t hu_value, float window_min, float window_max)
{
  float normalized_value = (static_cast<float>(hu_value) - window_min) / (window_max - window_min);
  normalized_value       = std::clamp(normalized_value, 0.0f, 1.0f);
  return static_cast<uint8_t>(normalized_value * 255.0f);
}

static void run_case(int case_id)
{
  const std::string nii_path    = k_img_dir + "/" + std::to_string(case_id) + ".img.nii";
  const std::string csv_path    = k_cl_dir + "/" + std::to_string(case_id) + ".csv";
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

  image2d<uint8_t> windowed_image(cmpr_image.width(), cmpr_image.height());
  for (int y = 0; y < cmpr_image.height(); ++y)
    for (int x = 0; x < cmpr_image.width(); ++x)
      windowed_image(x, y) = apply_window_level(cmpr_image(x, y), -200.0f, 800.0f);

  io::imsave(output_path.c_str(), windowed_image);
}

static void run_case_20rotation(int case_id)
{
  const std::string nii_path    = k_img_dir + "/" + std::to_string(case_id) + ".img.nii";
  const std::string csv_path    = k_cl_dir + "/" + std::to_string(case_id) + ".csv";

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

  volume<int16_t> scan_volume = read_scan_volume(nii_path);
  centerline vessel_centerline = load_centerline_csv(csv_path);

  if (vessel_centerline.size() < 2)
  {
    std::cerr << "Invalid centerline.\n";
    return;
  }

  const double voxel_spacing_mm = std::abs(scan_volume.spacing().x());

  constexpr int nb_angles = 20;
  constexpr double pi = 3.14159265358979323846;
  for (int i = 0; i < nb_angles; ++i)
  {
    const double angle = 2.0 * pi * static_cast<double>(i) / nb_angles;

    image2d<int16_t> cmpr = stretched(vessel_centerline, scan_volume,{std::cos(angle), std::sin(angle), 0.0}, 50, voxel_spacing_mm);
    image2d<uint8_t> output(cmpr.width(), cmpr.height());

    for (int y = 0; y < cmpr.height(); ++y)
      for (int x = 0; x < cmpr.width(); ++x)
        output(x, y) = apply_window_level(cmpr(x, y), -200.0f, 800.0f);

    const std::string output_path = std::format("{:02d}_id_projection_{:02d}.png",case_id, i);

    io::imsave(output_path.c_str(), output);
  }
}

TEST(CMPR_Real, Case01)
{
  for (int i = 100; i < 115; i++)
    run_case(i);
  // run_case_20rotation(100);
}
