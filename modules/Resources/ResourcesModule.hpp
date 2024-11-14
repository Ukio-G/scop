#ifndef __RESOURCESMODULE__H__
#define __RESOURCESMODULE__H__

#include "Graphic/Object3D.hpp"
#include "Graphic/Orbit.hpp"
#include "ModuleDispatcher/IModule.hpp"
#include "ModuleDispatcher/ModuleInfo.hpp"
#include "GeometryKeeper.hpp"
#include "modules/Resources/TexturesKeeper.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class ResourcesModuleInfo;

class ResourcesModule : public IModule {
    public:
    ResourcesModule() : IModule("ResourcesModule") {}
    using ModuleInfo = ResourcesModuleInfo;
    ~ResourcesModule();

    void init();
    void initEvents();
    void pushObject3D(Object3D* objPtr);
    void pushObjects3D(const std::vector<Object3D*> objPtr);
    
    private:
    GeometryKeeper::Ptr geometryKeeper = nullptr;;
    TexturesKeeper::Ptr textureKeeper = nullptr;;
    
    std::unordered_map<std::string, Object3D*> objects3D;

    Orbit::Ptr orbit = nullptr;
};

class ResourcesModuleInfo : public ModuleInfo {
    public:
    ResourcesModuleInfo() : ModuleInfo("ResourcesModule", []()->IModule*{return new ResourcesModule();}) {}
};


#endif  //!__RESOURCESMODULE__H__