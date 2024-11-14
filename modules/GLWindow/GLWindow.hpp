#ifndef __GLWINDOW__H__
#define __GLWINDOW__H__

#include "IO/Window.hpp"
#include "ModuleDispatcher/IModule.hpp"
#include "ModuleDispatcher/ModuleInfo.hpp"
#include "modules/Resources/GeometryKeeper.hpp"


class GLWindowModuleInfo;

class GLWindowModule : public IModule {
    public:
    using ModuleInfo = GLWindowModuleInfo;
    GLWindowModule() : IModule("GLWindowModule") {}
    ~GLWindowModule();
    void init();
    void initEvents();
    void configure();
    void start();

    private:
    Window* w;
    nlohmann::json controllersConfig;
};

class GLWindowModuleInfo : public ModuleInfo {
    public:
    GLWindowModuleInfo() : ModuleInfo("GLWindowModule", []() -> IModule* {return new GLWindowModule(); }) { }
};

#endif  //!__GLWINDOW__H__