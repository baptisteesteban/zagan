#pragma once

#include <zagan/core/image/image3d.hpp>

#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>

namespace zagan::io
{
  namespace detail
  {
    enum nifti_datatype : int16_t
    {
      DT_UINT8   = 2,
      DT_INT16   = 4,
      DT_INT32   = 8,
      DT_FLOAT32 = 16,
      DT_FLOAT64 = 64,
    };

#pragma pack(push, 1)
    struct nifti1_header
    {
      int32_t sizeof_hdr;
      char    data_type[10];
      char    db_name[18];
      int32_t extents;
      int16_t session_error;
      char    regular;
      char    dim_info;
      int16_t dim[8];     // offset 40, dim[0] is number of dimensions, dim[1..7] are size of each dimension
      char    _pad0[14];  // offset 56
      int16_t datatype;   // offset 70
      int16_t bitpix;     // offset 72
      char    _pad1[34];  // offset 74
      float   vox_offset; // offset 108, min 352
      char    _pad2[232]; // offset 112
      char    magic[4];   // offset 344, "n+1\0"
    };
#pragma pack(pop)

    static_assert(sizeof(nifti1_header) == 348, "NIfTI-1 header must be exactly 348 bytes");

    inline nifti1_header read_header(std::ifstream& file)
    {
      if (!file)
        throw std::runtime_error("file stream is not open");
      nifti1_header hdr;
      file.read(reinterpret_cast<char*>(&hdr), sizeof(nifti1_header));
      if (!file)
        throw std::runtime_error("failed to read NIfTI header");
      if (hdr.sizeof_hdr != 348)
        throw std::runtime_error("invalid NIfTI header: sizeof_hdr is " + std::to_string(hdr.sizeof_hdr) +
                                 ", expected 348");
      return hdr;
    }

    inline void validate_header(const nifti1_header& hdr)
    {
      if (std::memcmp(hdr.magic, "n+1\0", 4) != 0)
        throw std::runtime_error("invalid NIfTI file: magic field is not 'n+1\\0'");
      if (hdr.vox_offset < 352.0f)
        throw std::runtime_error("invalid vox_offset: " + std::to_string(hdr.vox_offset));
      if (hdr.dim[0] != 3)
        throw std::runtime_error("unsupported NIfTI file: only 3D volumes are supported");
      if (hdr.dim[1] <= 0 || hdr.dim[2] <= 0 || hdr.dim[3] <= 0)
        throw std::runtime_error("invalid dim: " + std::to_string(hdr.dim[1]) + "x" + std::to_string(hdr.dim[2]) + "x" +
                                 std::to_string(hdr.dim[3]));
      switch (hdr.datatype)
      {
      case DT_UINT8:
      case DT_INT16:
      case DT_INT32:
      case DT_FLOAT32:
      case DT_FLOAT64:
        break;
      default:
        throw std::runtime_error("unsupported NIfTI datatype: " + std::to_string(hdr.datatype));
      }
    }

    inline void read_voxel_data(std::ifstream& file, float vox_offset, void* dst, std::size_t n_bytes)
    {
      if (!file)
        throw std::runtime_error("file stream is not open");
      file.seekg(static_cast<std::streamoff>(std::llroundf(vox_offset)), std::ios::beg);
      if (!file)
        throw std::runtime_error("failed to seek to voxel data at offset " + std::to_string(vox_offset));
      file.read(reinterpret_cast<char*>(dst), n_bytes);
      if (!file)
        throw std::runtime_error("failed to read voxel data");
    }

  } // namespace detail

  template <typename T>
  image3d<T> read_nifti(const std::string& path)
  {
    std::ifstream file(path, std::ios::binary);
    if (!file)
      throw std::runtime_error("cannot open file: " + path);

    auto hdr = detail::read_header(file);
    detail::validate_header(hdr);

    const int nx = hdr.dim[1];
    const int ny = hdr.dim[2];
    const int nz = hdr.dim[3];

    const std::size_t bytes_per_voxel = static_cast<std::size_t>(hdr.bitpix) / 8;
    if (sizeof(T) != bytes_per_voxel)
      throw std::runtime_error("datatype mismatch: header specifies " + std::to_string(bytes_per_voxel) +
                               " bytes per voxel, but T has size " + std::to_string(sizeof(T)));

    image3d<T>        img(nx, ny, nz);
    const std::size_t n_bytes = static_cast<std::size_t>(nx) * ny * nz * bytes_per_voxel;
    detail::read_voxel_data(file, hdr.vox_offset, img.buffer(), n_bytes);

    return img;
  }

} // namespace zagan::io
