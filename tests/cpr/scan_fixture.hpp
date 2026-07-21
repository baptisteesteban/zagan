#pragma once

#include <zagan/core/image/image2d.hpp>
#include <zagan/core/volume/volume.hpp>
#include <zagan/cpr/centerline.hpp>
#include <zagan/io/nifti_io.hpp>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace zagan::testing
{
  inline const std::filesystem::path k_project_root =
      std::filesystem::path(__FILE__).parent_path().parent_path().parent_path();
  inline const std::string k_img_dir = (k_project_root / "data/3dimg/extracted").string();
  inline const std::string k_cl_dir  = (k_project_root / "data/pfee/centerlines/centerlines").string();

  inline std::string scan_path(int case_id)
  {
    return k_img_dir + "/" + std::to_string(case_id) + ".img.nii";
  }
  inline std::string centerline_path(int case_id)
  {
    return k_cl_dir + "/" + std::to_string(case_id) + ".csv";
  }

  inline std::vector<int> available_cases()
  {
    std::vector<int> ids;
    std::error_code  ec;
    for (const auto& entry : std::filesystem::directory_iterator(k_img_dir, ec))
    {
      const std::string name = entry.path().filename().string();
      const auto        dot  = name.find(".img.nii");
      if (dot == std::string::npos || dot == 0)
        continue;
      const std::string stem = name.substr(0, dot);
      if (stem.find_first_not_of("0123456789") != std::string::npos)
        continue;
      const int id = std::stoi(stem);
      if (std::filesystem::exists(centerline_path(id)))
        ids.push_back(id);
    }
    std::sort(ids.begin(), ids.end());
    return ids;
  }

  inline volume<int16_t> read_scan_volume(const std::string& path)
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
    image3d<int16_t> img = io::read_nifti<int16_t>(path).image;

    volume<int16_t>   vol(img.width(), img.height(), img.depth(), spacing, origin);
    const std::size_t n = static_cast<std::size_t>(img.width()) * img.height() * img.depth();
    std::copy(img.buffer(), img.buffer() + n, vol.buffer());
    return vol;
  }

  inline cpr::centerline load_centerline_csv(const std::string& csv_path)
  {
    std::ifstream csv_file(csv_path);
    std::string   line;
    std::getline(csv_file, line);
    std::vector<vec3>   points;
    std::vector<double> radii;
    while (std::getline(csv_file, line))
    {
      if (line.empty())
        continue;
      std::istringstream line_stream(line);
      std::string        token;
      std::getline(line_stream, token, ',');
      std::getline(line_stream, token, ',');
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
    return cpr::centerline(std::move(points), std::move(radii));
  }

  inline uint8_t apply_window_level(int16_t hu_value, float window_min, float window_max)
  {
    float normalized_value = (static_cast<float>(hu_value) - window_min) / (window_max - window_min);
    normalized_value       = std::clamp(normalized_value, 0.0f, 1.0f);
    return static_cast<uint8_t>(normalized_value * 255.0f);
  }

  inline constexpr float k_window_min = -200.0f;
  inline constexpr float k_window_max = 800.0f;

  template <typename T>
  image2d<uint8_t> window_to_gray(const image2d<T>& src)
  {
    image2d<uint8_t> dst(src.width(), src.height());
    for (int y = 0; y < src.height(); ++y)
      for (int x = 0; x < src.width(); ++x)
        dst(x, y) = apply_window_level(src(x, y), k_window_min, k_window_max);
    return dst;
  }

  template <typename T>
  image2d<uint8_t> window_slice_to_gray(const image3d<T>& src, int z)
  {
    image2d<uint8_t> dst(src.width(), src.height());
    for (int y = 0; y < src.height(); ++y)
      for (int x = 0; x < src.width(); ++x)
        dst(x, y) = apply_window_level(static_cast<int16_t>(src(x, y, z)), k_window_min, k_window_max);
    return dst;
  }
} // namespace zagan::testing
