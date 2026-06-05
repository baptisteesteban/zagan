#include <zagan/io/imsave.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <format>
#include <stdexcept>
#include <string_view>

namespace zagan::io
{
  template <>
  void imsave<std::uint8_t>(const char* filename, const image2d<std::uint8_t>& img)
  {
    if (!std::string_view(filename).ends_with(".png"))
      throw std::invalid_argument(std::format("Input image must be .png (Got {})", filename));
    stbi_write_png(filename, img.width(), img.height(), 1, img.buffer(), img.width());
  }
} // namespace zagan::io