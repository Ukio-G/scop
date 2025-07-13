// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "ResourcesModule.hpp"
#include "Graphic/Line.hpp"
#include "Graphic/Object3D.hpp"
#include "modules/Resources/DescriptionObject3D.hpp"
#include "modules/Resources/GeometryKeeper.hpp"
#include "modules/Resources/TexturesKeeper.hpp"
#include <any>
#include <memory>
#include <string>

inline std::vector<glm42::vec3> genCircle(int segnemts) {
    std::vector<glm42::vec3> result(segnemts);
    float theta = 2 * 3.1415926 / float(segnemts); 
    result.reserve(segnemts);
    glm42::mat3 rot = glm42::rotate(glm42::mat4(1.0), theta, {0.0, 1.0, 0.0});
    glm42::vec3 prev = {0.0, 0.0, 1.0};
    
    result[0] = {prev[0], prev[1], prev[2]};

    for (int i = 1; i < segnemts; i++) {
        glm42::vec3 next = prev * rot;
        result[i] = {next[0], next[1], next[2]};
        prev = next;
    }
    
    return result;
};

void ResourcesModule::init() {
    if (inited)
        return;

    geometryKeeper = std::make_shared<GeometryKeeper>();
    textureKeeper = std::make_shared<TexturesKeeper>();

    std::string basedir = cfg["resourcesDir"];
    std::string objectsJSON = basedir + static_cast<std::string>(cfg["objectsConfiguration"]);

    std::string modelsDir = basedir + static_cast<std::string>(cfg["modelsDir"]);
    std::string texturesDir = basedir + static_cast<std::string>(cfg["texturesDir"]);


    inited = true;
}

void ResourcesModule::initEvents() {
    if (eventsInited)
        return;

    subscriber->addActionToTopic(getConfigurationEventName(), [this](std::any data){
        std::cout << "["<< name << "] cfg received" << std::endl;
    });

    subscriber->addActionToTopic("getOrbit", [this](std::any data) {
        auto& f = std::any_cast<std::function<void(Orbit::Ptr)>&>(data);
        f(orbit);
    });

    EventChannel::getInstance().subscribe(getConfigurationEventName(), subscriber);
    EventChannel::getInstance().subscribe("getOrbit", subscriber);

    eventsInited = true;
}

ResourcesModule::~ResourcesModule() { }

void ResourcesModule::pushObject3D(Object3D* objPtr) {
    EventChannel::getInstance().publish("NewGeometry", objPtr);
}

void ResourcesModule::pushObjects3D(const std::vector<Object3D*> vecObjPtr) {
    EventChannel::getInstance().publish("NewGeometry", vecObjPtr);
}