.PHONY: all clean
CXXFLAGS = -std=c++20 -O3 #-Wextra -Wall -Werror
INCLUDE = -I. -Imodules/GLWindow -Imodules/Resources -Iinclude -Iinclude/Graphic -Iinclude/IO -Iinclude/ModuleDispatcher -I./deps/include
CXX=clang++

# Cross platform section
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)          # macOS
  SYS_LIBS := -L./deps/lib -framework OpenGL -framework Cocoa
  CXXFLAGS += -DGL_SILENCE_DEPRECATION
else ifeq ($(UNAME_S),Linux)      # Linux
  SYS_LIBS := -L./deps/lib
endif

LD_FLAGS = -Wl,-rpath,'$$ORIGIN/deps/lib' -Wl,-z,origin $(SYS_LIBS) -lGL -lGLEW -lglfw

# Create list of .obj files, based on .c files
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

scop: $(OBJS)
	$(CXX) $(LD_FLAGS) $(CXXFLAGS) $(INCLUDE) $(OBJS) -o $@

clean:
	rm -rf $(OBJS_DIR)

all: clean scop
