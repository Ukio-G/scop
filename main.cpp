// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "IO/Window.hpp"
#include "cfg_parser.hpp"
#include "modules/Resources/TexturesKeeper.hpp"
#include <cstring>
#include <filesystem>
#include <memory>
#include "EventChannel.hpp"

#include "TextureParser.hpp"

std::unique_ptr<config> config_ptr = nullptr;

struct Context {
  GeometryKeeper *geometryKeeper = nullptr;
  Window * window = nullptr;
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
    textures = &ctx.texturesKeeper->textures.begin()->second;
  }

  ctx.window->addObject3DToDraw(new Object3D(name, geometry, textures));
}

void initDrawResources(Context & ctx)
{
  auto &cfg = *( config_ptr.get() );

  for( auto &[ name, section ] : cfg.sections ) {
    if( name == "config" )
      continue;
    try {
      if ( !section.contains("render") )
        continue;

      bool is_render = std::get< bool >( section.at( "render" ) );
      if( !is_render )
        continue;

      auto        model_name = std::get< std::string >( section.at( "name" ) );
      auto        model_path = std::get< std::string >( section.at( "file_path" ) );
      std::string texture_path;

      if( section.contains( "texture" ) ) {
        texture_path = std::get< std::string >( section.at( "texture" ) );
        std::cout << "Loading texture: " << texture_path << std::endl;
      }

      addToDraw( ctx, model_name, model_path, texture_path );
    } catch( std::invalid_argument &argument ) {
      std::cout << "error handle section - no such section, skip " << name << std::endl;
    } catch( std::bad_variant_access &bad_variant_access ) {
      std::cout << "error handle section - invalid variant cast, skip " << name << std::endl;
    } catch( std::out_of_range &out_of_range ) {
      std::cout << "error handle section - out_of_range (no such field in section), skip " << name << std::endl;
    }
  }
}

void handleCommandLine(int argc, char **argv) {
  if( argc == 3 && strcmp( argv[ 1 ], "-c" ) == 0 ) {

    if ( !std::filesystem::exists(argv[ 2 ]) )
    {
      throw std::runtime_error( "Invalid config file. File not exist" );
    }
    
    if ( std::filesystem::path(argv[ 2 ]).extension() != ".cfg" )
    {
      throw std::runtime_error( "Invalid config file. File not .cfg" );
    }

    config_ptr = std::make_unique< config >( argv[ 2 ] );
  } 
  else 
  {
    throw std::runtime_error( "Invalid launch options. Expected: scop -c <config>" );
  }
}

int main(int argc, char **argv) {
  Context ctx;
  try {
    handleCommandLine(argc, argv);
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }
  GeometryKeeper g_keeper;
  TexturesKeeper t_keeper;

  ctx.geometryKeeper = &g_keeper;
  ctx.texturesKeeper = &t_keeper;

  auto & windows_settings = config_ptr->sections["window_settings"];
  
  long width, height;
  width  << windows_settings["width"];
  height << windows_settings["height"];
  
  Window w( width, height );
  ctx.window = &w;
  w.init();

  initDrawResources(ctx);

  w.drawLoop();
  EventChannel::destroy();

  return 0;
}
