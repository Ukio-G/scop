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

    std::string basedir = config["resourcesDir"];
    std::string objectsJSON = basedir + static_cast<std::string>(config["objectsConfiguration"]);

    std::string modelsDir = basedir + static_cast<std::string>(config["modelsDir"]);
    std::string texturesDir = basedir + static_cast<std::string>(config["texturesDir"]);

    nlohmann::json objectsConfiguration;
    std::ifstream ifs(objectsJSON);
    if (ifs.fail()){
        throw std::runtime_error("[ResourcesModule] JSON file " + objectsJSON + " not found");
    }

    objectsConfiguration = nlohmann::json::parse(ifs);

    for (auto & object : objectsConfiguration) {
        DescriptionObject3D descriptionObject = object;

        std::cout << descriptionObject;

        geometryKeeper->loadGeometryFromFile(descriptionObject.modelPath, basedir + descriptionObject.modelPath);
        textureKeeper->loadTexturesFromFile(descriptionObject, basedir);
        
        // Possible leak if object already exist
        if (objects3D.find(descriptionObject.name) != objects3D.end()) {
            delete objects3D[descriptionObject.name];
        }

        objects3D[descriptionObject.name] = new Object3D(geometryKeeper->geometry[descriptionObject.modelPath]);
        objects3D[descriptionObject.name]->textures = &(textureKeeper->textures[descriptionObject.name]);

        if (descriptionObject.translate)
            objects3D[descriptionObject.name]->setTranslate(*descriptionObject.translate);

        if (descriptionObject.rotate)
            objects3D[descriptionObject.name]->setRotate(*descriptionObject.rotate);

        if (descriptionObject.scale)
            objects3D[descriptionObject.name]->setScale(*descriptionObject.scale);
        
        objects3D[descriptionObject.name]->updateModelMatrix();
        
        pushObject3D(objects3D[descriptionObject.name]);
    }

    geometryKeeper->newLineGeometry("Line", {0.0, -15.0, 0.0}, {0.0, 15.0, 0.0});
    geometryKeeper->newLineStripGeometry("Orbit", genCircle(100));

    Line l = geometryKeeper->instance<Line>("Line");
    LineStrip ls = geometryKeeper->instance<LineStrip>("Orbit");

    auto moon_it = objects3D.find("Moon");
    if (moon_it != objects3D.end()) {
        orbit = std::make_shared<Orbit>(moon_it->second);
        orbit->axis = l;
        orbit->orbit = ls;
    }
    else
        std::cout << "WARNING! Moon not found." << std::endl;

    inited = true;
}

void ResourcesModule::initEvents() {
    if (eventsInited)
        return;

    subscriber->addActionToTopic(getConfigurationEventName(), [this](std::any data){
        std::cout << "["<< name << "] config received" << std::endl;
        config = std::any_cast<nlohmann::json>(data);
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