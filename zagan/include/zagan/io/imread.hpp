#pragma once

#include <zagan/core/image/image2d.hpp>

namespace zagan::io
{
  template <typename T>
  void imread(const char* filename, image2d<T>& img);
}