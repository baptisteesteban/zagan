#pragma once

#include <zagan/core/image/image3d.hpp>

#include <memory>

#include <nifti1_io.h>

namespace zagan::io
{

  template <typename T>
  struct nifti_volume
  {
    image3d<T>                                           image;
    std::unique_ptr<nifti_image, void (*)(nifti_image*)> meta;
  };

} // namespace zagan::io
