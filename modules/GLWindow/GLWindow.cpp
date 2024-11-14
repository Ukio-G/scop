#include "GLWindow.hpp"
#include "BuffersCollection.hpp"
#include "EventChannel.hpp"
#include "Graphic/Object3D.hpp"
#include "Camera.hpp"
#include <any>
#include <vector>

GLWindowModule::~GLWindowModule() { }

void GLWindowModule::initEvents() {
    if (eventsInited)
        return; 
    
    subscriber->addActionToTopic(getConfigurationEventName(), [this](std::any data){
        std::cout << "["<< name << "] config received" << std::endl;
        config = std::any_cast<nlohmann::json>(data);
    });

    subscriber->addActionToTopic("NewGeometry", [this](std::any data){
        std::cout << "["<< name << "] NewGeometry received" << std::endl;
        w->addObject3DToDraw(std::any_cast<Object3D*>(data));
    });

    subscriber->addActionToTopic("NewGeometryVector", [this](std::any data){
        std::cout << "["<< name << "] NewGeometryVector received" << std::endl;
        auto& geomentyVector = std::any_cast<std::vector<Object3D*>&>(data);
        for(auto& object3DPtr: geomentyVector) {
            w->addObject3DToDraw(object3DPtr);
        }
    });

    EventChannel::getInstance().subscribe(getConfigurationEventName(), subscriber);
    EventChannel::getInstance().subscribe("NewGeometry", subscriber);
    EventChannel::getInstance().subscribe("NewGeometryVector", subscriber);
    eventsInited = true;
}


void GLWindowModule::configure() {
    if (configured)
        return;
    
    waitConfigurationReceived();

    w = new Window(config["width"], config["height"]);
    w->camera->position = {config["cameraPosition"][0], config["cameraPosition"][1], config["cameraPosition"][2]};
    configured = true;
}

void GLWindowModule::init() { 
    if (inited)
        return;     
    
    inited = true;
}

void GLWindowModule::start() {
    std::cout << "[GLWindowModule] start" << std::endl;
    w->startDrawing();
}