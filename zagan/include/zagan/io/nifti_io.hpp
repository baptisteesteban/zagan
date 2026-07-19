#pragma once

#include <zagan/io/nifti_volume.hpp>

#include <cstdint>
#include <string>

namespace zagan::io
{

  template <typename T>
  nifti_volume<T> read_nifti(const std::string& path);

  template <typename T>
  void write_nifti(const std::string& path, nifti_volume<T>& vol);

  extern template nifti_volume<std::int16_t> read_nifti<std::int16_t>(const std::string&);

  extern template void write_nifti<std::int16_t>(const std::string&, nifti_volume<std::int16_t>&);

} // namespace zagan::io
