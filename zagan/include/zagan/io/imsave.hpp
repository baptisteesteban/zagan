#pragma once

#include <zagan/core/image/image2d.hpp>

namespace zagan::io
{
  template <typename T>
  void imsave(const char* filename, const image2d<T>& img);
}