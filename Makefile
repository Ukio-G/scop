.PHONY: all clean
# miniRT compilation flags
CFLAGS = -std=c++20 #-Wextra -Wall -Werror
INCLUDE = -I. -Imodules/GLWindow -Imodules/Resources -Iinclude -Iinclude/Graphic -Iinclude/IO -Iinclude/ModuleDispatcher -I/opt/homebrew/include
LD_FLAGS += -L. -L/opt/homebrew/lib -lglew -lglfw3 -framework OpenGL -framework Cocoa
CXX=clang++

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
#src/TransformBehaviour/TransformBehaviour.cpp \
#modules/Resources/ResourcesModule.cpp \

OBJS = $(addprefix $(OBJS_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(SRCS))))

$(addprefix $(OBJS_DIR)/,%.o): $(addprefix src/, $(notdir %.cpp)) $(HEADERS)
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(addprefix $(OBJS_DIR)/,%.o): $(addprefix modules/Resources/, $(notdir %.cpp)) $(HEADERS)
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(addprefix $(OBJS_DIR)/,%.o): $(addprefix src/TransformBehaviour/, $(notdir %.cpp)) $(HEADERS)
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< -o $@


$(addprefix $(OBJS_DIR)/,%.o): $(addprefix src/IO/, $(notdir %.cpp)) $(HEADERS)
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< -o $@


$(addprefix $(OBJS_DIR)/,%.o): $(addprefix ./, $(notdir %.cpp)) $(HEADERS)
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< -o $@

scop: $(OBJS)
	$(CXX) $(CFLAGS) $(INCLUDE) $(OBJS) -o $@

clean:
	rm -rf $(OBJS_DIR)

all: clean scop
