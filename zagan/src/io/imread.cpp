#include <zagan/colors/rgb.hpp>
#include <zagan/io/imread.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <format>
#include <stdexcept>

namespace zagan::io
{
  template <>
  void imread<std::uint8_t>(const char* filename, image2d<std::uint8_t>& img)
  {
    int           width, height, nchan;
    std::uint8_t* buffer = stbi_load(filename, &width, &height, &nchan, 1);
    if (!buffer)
      throw std::runtime_error(std::format("Unable to read image {}", filename));
    if (nchan != 1)
      throw std::invalid_argument(std::format("Invalid number of channel (Got {}, expected 1)", nchan));

    img.resize(width, height);
    for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
        img(x, y) = buffer[y * width + x];
    }

    stbi_image_free(buffer);
  }

  template <>
  void imread<colors::rgb8>(const char* filename, image2d<colors::rgb8>& img)
  {
    int           width, height, nchan;
    std::uint8_t* buffer = stbi_load(filename, &width, &height, &nchan, 3);
    if (!buffer)
      throw std::runtime_error(std::format("Unable to read image {}", filename));
    if (nchan != 3)
      throw std::invalid_argument(std::format("Invalid number of channel (Got {}, expected 3)", nchan));

    img.resize(width, height);
    const int stride = width * 3;
    for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
      {
        img(x, y).red()   = buffer[y * stride + (x * 3)];
        img(x, y).green() = buffer[y * stride + (x * 3) + 1];
        img(x, y).blue()  = buffer[y * stride + (x * 3) + 2];
      }
    }
    stbi_image_free(buffer);
  }
} // namespace zagan::io