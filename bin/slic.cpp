#include <zagan/io/imread.hpp>
#include <zagan/io/imsave.hpp>
#include <zagan/labeling/slic.hpp>

int main(int argc, char* argv[])
{
  if (argc < 3)
  {
    std::cout << "Usage: " << argv[0] << " input output\n";
    return 1;
  }

  using namespace zagan;
  image2d<colors::rgb8> img;
  io::imread(argv[1], img);
  auto out = labeling::slic<std::uint8_t>(img, 150);
  io::imsave("out.png", out);

  return 0;
}