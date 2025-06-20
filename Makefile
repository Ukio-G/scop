.PHONY: all clean
# miniRT compilation flags
CFLAGS = -std=c++20 #-Wextra -Wall -Werror
INCLUDE = -I. -Imodules/GLWindow -Imodules/Resources -Iinclude -Iinclude/Graphic -Iinclude/IO -Iinclude/ModuleDispatcher

LD_FLAGS += -L. -fPIC -rdynamic
CXX=g++

# Create list of .obj files, based on .c files
OBJS_DIR = objs

SRCS = main.cpp src/Camera.cpp \
src/KeysControls.cpp \
src/mouseHandler.cpp \
src/Object3D.cpp \
src/Orbit.cpp \
src/Shader.cpp \
src/Texture.cpp \
src/Window.cpp \
modules/Resources/GeometryKeeper.cpp \
modules/Resources/ResourcesModule.cpp \
modules/Resources/TextureKeeper.cpp \
modules/GLWindow/GLWindow.cpp


OBJS = $(addprefix $(OBJS_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(SRCS))))
#
$(addprefix $(OBJS_DIR)/,%o): main.cpp $(HEADERS)
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(addprefix $(OBJS_DIR)/,%.o): $(addprefix src/, $(notdir %.cpp)) $(HEADERS)
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(addprefix $(OBJS_DIR)/,%.o): $(addprefix modules/GLWindow, $(notdir %.cpp)) $(HEADERS)
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(addprefix $(OBJS_DIR)/,%.o): $(addprefix modules/Resources, $(notdir %.cpp)) $(HEADERS)
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< -o $@


scop: $(OBJS)
	$(CXX) $(CFLAGS) $(INCLUDE) $(OBJS) -o $@

clean:
	rm -rf $(OBJS_DIR)

all: clean scop