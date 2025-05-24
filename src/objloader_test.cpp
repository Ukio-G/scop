#include "objLoader.hpp"
#include <array>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char **argv)
{

  std::string input = "   v  1        43";
  auto offset = obj::Loader::skip_spaces(input);
  auto array = obj::Loader::read_n_values<int, 4>(input, offset + 1);
  for (auto &item : array) {
    std::cout << item << " ";
  }

  std::cout << std::endl;

  //  std::string path(argv[1]);
//  obj::Loader loader;




//  loader.LoadFile(path);

  return 0;
}