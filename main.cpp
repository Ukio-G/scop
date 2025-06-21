#include "IO/Window.hpp"
#include "cfg_parser.hpp"
#include "modules/Resources/TexturesKeeper.hpp"
#include <cstring>
#include <filesystem>

#include "TextureParser.hpp"

struct Context {
  GeometryKeeper *geometryKeeper = nullptr;
  Window * window = nullptr;
  config * cfg = nullptr;
  TexturesKeeper * texturesKeeper = nullptr;
};

void addToDraw(Context & ctx, const std::string & name, const std::string & filepath)
{
  ctx.geometryKeeper->loadGeometryFromFile(name, filepath);
  TextureParser parser(ctx.texturesKeeper);
  parser.loadBMPFromFile("../resources/texture.bmp");

  Object3D *obj = new Object3D;
  obj->geometry = ctx.geometryKeeper->geometry[name];
  obj->name = name;
  obj->textures = &ctx.texturesKeeper->textures["texture"];
  ctx.window->addObject3DToDraw(obj);
}

void handleCommandLine(int argc, char **argv, Context & ctx) {
  if (strcmp(argv[1], "-c") == 0 && argc == 3) {
    config cfg(argv[2]);

    for (auto&[name, section] : cfg.sections) {
      if (name == "config")
        continue;
      try {
        bool is_render = std::get<bool>(section.at("render"));
        if (!is_render)
          continue;

        auto model_name = std::get<std::string>(section.at("name"));
        auto model_path = std::get<std::string>(section.at("file_path"));

        addToDraw(ctx, model_name, model_path);
      } catch (std::invalid_argument &argument) {
        std::cout << "error handle section - no such section, skip " << name << std::endl;
      } catch (std::bad_variant_access &bad_variant_access) {
        std::cout << "error handle section - invalid variant cast, skip " << name << std::endl;
      }
      catch (std::out_of_range &out_of_range) {
        std::cout << "error handle section - out_of_range (no such field in section), skip " << name << std::endl;

      }
    }
  }
  else if (argc == 2) {
    std::string model_path(argv[2]);
    std::string model_name = std::filesystem::path(model_path).string();
    addToDraw(ctx, model_name, model_path);
  }
  else {
    throw std::runtime_error("invalid launch options");
  }
}

int main(int argc, char **argv) {
  Context ctx;

  Window w(1800, 1600);
  ctx.window = &w;

  GeometryKeeper g_keeper;
  TexturesKeeper t_keeper;

  ctx.geometryKeeper = &g_keeper;
  ctx.texturesKeeper = &t_keeper;

  handleCommandLine(argc, argv, ctx);

  w.drawLoop();

  return 0;
}
