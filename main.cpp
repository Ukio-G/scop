#include "IO/Window.hpp"
#include "cfg_parser.hpp"
#include "modules/Resources/TexturesKeeper.hpp"

int main(int argc, char **argv) {
  Window w(800, 600);

  GeometryKeeper g_keeper;
  TexturesKeeper t_keeper;

  config cfg(argv[1]);

  auto model = cfg["model"];
  auto name = std::get<std::string>(model["name"]);
  auto file_path = std::get<std::string>(model["file_path"]);
  g_keeper.loadGeometryFromFile(name, file_path);

  Object3D obj;
  obj.geometry = g_keeper.geometry[name];
  obj.name = name;

  w.addObject3DToDraw(&obj);
  w.drawLoop();

  return 0;
}
