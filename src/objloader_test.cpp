#include "objLoader.hpp"

int main(int argc, char **argv)
{
  obj::Loader loader;
  loader.LoadFile(argv[1]);

//  loader.printDebug();


  return 0;
}