#include <zagan/io/imread.hpp>
#include <zagan/io/imsave.hpp>
#include <zagan/labeling/slic.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cerr << "Usage: " << argv[0] << " input K compactness\n";
    return 1;
  }

  using namespace zagan;
  image2d<colors::rgb8> img;
  io::imread(argv[1], img);
  int  K           = std::atoi(argv[2]);
  int  compactness = std::atoi(argv[3]);
  auto out         = labeling::slic<std::uint8_t>(img, K, compactness);
  io::imsave("out.png", out);

  return 0;
}