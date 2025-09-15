// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "IO/Window.hpp"
#include "cfg_parser.hpp"
#include "modules/Resources/TexturesKeeper.hpp"
#include <cstring>
#include <filesystem>
#include "EventChannel.hpp"

#include "TextureParser.hpp"

struct Context {
  GeometryKeeper *geometryKeeper = nullptr;
  Window * window = nullptr;
  config * cfg = nullptr;
  TexturesKeeper * texturesKeeper = nullptr;
};

void addToDraw(Context & ctx, const std::string & name, const std::string & filepath, const std::string & texture_path)
{
  ctx.geometryKeeper->loadGeometryFromFile(name, filepath);

  TexturesPack* textures = nullptr;
  Geometry geometry = ctx.geometryKeeper->geometry[name];
  if (!texture_path.empty())
  {
    TextureParser parser(ctx.texturesKeeper);

    parser.loadBMPFromFile( texture_path );
    textures = &ctx.texturesKeeper->textures["texture"];
  }

  ctx.window->addObject3DToDraw(new Object3D(name, geometry, textures));
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
        std::string texture_path;
        if ( section.contains( "texture" ) )
        {
          texture_path = std::get<std::string>(section.at("texture"));
          std::cout << "Loading texture: " << texture_path << std::endl;
        }

        addToDraw(ctx, model_name, model_path, texture_path);
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
    addToDraw(ctx, model_name, model_path, "");
  }
  else {
    throw std::runtime_error("invalid launch options");
  }
}

int main(int argc, char **argv) {
  Context ctx;

  Window w(1000, 1000);
  ctx.window = &w;

  GeometryKeeper g_keeper;
  TexturesKeeper t_keeper;

  ctx.geometryKeeper = &g_keeper;
  ctx.texturesKeeper = &t_keeper;

  handleCommandLine(argc, argv, ctx);

  w.drawLoop();
  EventChannel::destroy();

  return 0;
}
