#include <zagan/io/nifti_io.hpp>

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>

#include <nifti1_io.h>

namespace zagan::io
{

  template <typename T>
  nifti_volume<T> read_nifti(const std::string& path)
  {
    nifti_image* nim = nifti_image_read(path.c_str(), 1);
    if (!nim)
      throw std::runtime_error("failed to read NIfTI file: " + path);

    if (nim->ndim != 3)
    {
      nifti_image_free(nim);
      throw std::runtime_error("unsupported NIfTI file: only 3D volumes are supported");
    }
    if (static_cast<int>(sizeof(T)) != nim->nbyper)
    {
      nifti_image_free(nim);
      throw std::runtime_error("datatype mismatch: header specifies " + std::to_string(nim->nbyper) +
                               " bytes per voxel, but T has size " + std::to_string(sizeof(T)));
    }

    nifti_volume<T> vol{image3d<T>(nim->nx, nim->ny, nim->nz), {nim, nifti_image_free}};

    const std::size_t n_bytes = static_cast<std::size_t>(nim->nx) * nim->ny * nim->nz * nim->nbyper;
    std::memcpy(vol.image.buffer(), nim->data, n_bytes);
    std::free(nim->data);
    nim->data = nullptr;

    return vol;
  }

  template <typename T>
  void write_nifti(const std::string& path, nifti_volume<T>& vol)
  {
    nifti_image* nim = vol.meta.get();
    if (nifti_set_filenames(nim, path.c_str(), 0, 1) != 0)
      throw std::runtime_error("failed to set NIfTI filenames for: " + path);
    nim->data = vol.image.buffer();
    nifti_image_write(nim);
    nim->data = nullptr;
  }

  template nifti_volume<std::int16_t> read_nifti<std::int16_t>(const std::string&);

  template void write_nifti<std::int16_t>(const std::string&, nifti_volume<std::int16_t>&);

} // namespace zagan::io
