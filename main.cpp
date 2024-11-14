#include "ModuleDispatcher/ModuleDispatcher.hpp"
#include "modules/GLWindow/GLWindow.hpp"
#include "modules/Resources/ResourcesModule.hpp"

std::optional<config> readFromFile(const std::string & path) {
    if (fileExist(path))
        return config(path);
    return std::nullopt;
}

int main(int argc, char ** argv) {
	if (argc != 2) {
		std::cerr << "Reqiered configuration file as first argument: ./opengl-sample <relative path to config.json>" << std::endl;
	}

	ModuleDispatcher md;
	md.registerModuleInfo(GLWindowModule::ModuleInfo());
	md.registerModuleInfo(ResourcesModule::ModuleInfo());

	//md.loadConfigurations(argv[1]);

	md.instanceModulesFromConfig();
	std::cout << "Modules instanced" << std::endl;

	md.initModulesEvents();
	std::cout << "Events inited" << std::endl;

	md.pushConfigToModules();

	md.configureModules();
	std::cout << "Modules configured" << std::endl;

	md.initModules();
	std::cout << "Modules initialized" << std::endl;

	md.startModules();

	md.execLoop();

	return 0;
}
