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
  }
} // namespace zagan::io