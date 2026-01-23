.PHONY: all clean
CXXFLAGS = -std=c++20 -O3 # -Wextra -Wall -Werror
INCLUDE = -I. -Imodules/GLWindow -Imodules/Resources -Iinclude -Iinclude/Graphic -Iinclude/IO -Iinclude/ModuleDispatcher -I./deps/include
CXX=g++

# Cross platform section
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)          # macOS
  SYS_LIBS := -L./deps/lib -Wl,-rpath,@executable_path -framework OpenGL -framework Cocoa
  CXXFLAGS += -DGL_SILENCE_DEPRECATION
else ifeq ($(UNAME_S),Linux)      # Linux
  SYS_LIBS := -Wl,-rpath,'$$ORIGIN/deps/lib' -fPIC -L./deps/lib -lGL
endif

LD_FLAGS = $(SYS_LIBS) -lGLEW -lglfw


EXECUTABLE:=scop
OBJS_DIR = objs

SRCS = ./main.cpp \
src/TextureParser.cpp \
src/Camera.cpp \
src/Window.cpp \
src/KeysControls.cpp \
src/mouseHandler.cpp \
src/IO/ShadersControls.cpp \
src/IO/ObjectsControls.cpp \
src/Texture.cpp \
src/Shader.cpp \
src/Object3D.cpp \
modules/Resources/GeometryKeeper.cpp \
modules/Resources/TextureKeeper.cpp

HEADERS = include/BuffersCollection.hpp \
include/Camera.hpp \
include/EventChannel.hpp \
include/EventLoop.hpp \
include/FPSCounter.hpp \
include/Graphic/Object3D.hpp \
include/Graphic/RenderUtils/WireBBoxManager.hpp \
include/Graphic/Shader.hpp \
include/Graphic/Texture.hpp \
include/Graphic/TextureParameters.hpp \
include/Graphic/TexturesPack.hpp \
include/IO/KeysControls.hpp \
include/IO/MouseEvents.hpp \
include/IO/ObjectsControls.hpp \
include/IO/ShadersControls.hpp \
include/IO/Window.hpp \
include/IO/WindowSettings.hpp \
include/IO/mouseHandler.hpp \
include/ModuleDispatcher/EventChannel.hpp \
include/ModuleDispatcher/EventLoop.hpp \
include/ModuleDispatcher/IModule.hpp \
include/ModuleDispatcher/ModuleDispatcher.hpp \
include/ModuleDispatcher/ModuleInfo.hpp \
include/TextureParser.hpp \
include/TransformBehaviour/TransformBehaviour.hpp \
include/Vertex.hpp \
include/cfg_parser.hpp \
include/math.hpp \
include/Debug3DLine.hpp \
modules/Resources/DescriptionObject3D.hpp \
modules/Resources/GeometryKeeper.hpp \
modules/Resources/ResourcesModule.hpp \
modules/Resources/TexturesKeeper.hpp
OBJS = $(addprefix $(OBJS_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(SRCS))))

$(addprefix $(OBJS_DIR)/,%.o): $(addprefix src/, $(notdir %.cpp)) $(HEADERS)
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(addprefix $(OBJS_DIR)/,%.o): $(addprefix modules/Resources/, $(notdir %.cpp)) $(HEADERS)
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(addprefix $(OBJS_DIR)/,%.o): $(addprefix src/TransformBehaviour/, $(notdir %.cpp)) $(HEADERS)
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(addprefix $(OBJS_DIR)/,%.o): $(addprefix src/IO/, $(notdir %.cpp)) $(HEADERS)
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(addprefix $(OBJS_DIR)/,%.o): $(addprefix ./, $(notdir %.cpp)) $(HEADERS)
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(EXECUTABLE): $(OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(OBJS) $(LD_FLAGS) -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(EXECUTABLE)

re: fclean all

all: $(EXECUTABLE)

.PHONY: all clean re fclean
