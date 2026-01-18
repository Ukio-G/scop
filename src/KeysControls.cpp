// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "IO/KeysControls.hpp"

KeysControls::KeysControls(Window &window) : window(&window) {
	eventSubscriber = Subscriber::create("KeysControlsSubscriber");
	EventChannel::getInstance().subscribe("NewKeyEvent", eventSubscriber);
	eventSubscriber->addActionToTopic("NewKeyEvent", "KeyPressed",[&](Subscriber::ActionArgument argument) {
		auto keyEvent = std::any_cast<std::pair<KeyType, ActionType>>(argument);
		actions[keyEvent.first] = keyEvent.second;
	});

	EventChannel::getInstance().subscribe("NewKeyReleaseEvent", eventSubscriber);
	eventSubscriber->addActionToTopic("NewKeyReleaseEvent", "KeyReleased",[&](Subscriber::ActionArgument argument) {
		auto keyEvent = std::any_cast<std::pair<KeyType, ActionType>>(argument);
		actionsReleased[keyEvent.first] = keyEvent.second;
	});
}

void KeysControls::pollingKeysEvent() {
	for (auto &item: actions) {
		auto key = item.first;
		if (glfwGetKey(window->glfwWindow, key) == GLFW_PRESS)
			item.second(window);
	}

	for (auto& item : actionsReleased) {
		auto key = item.first;
		int s = glfwGetKey(window->glfwWindow, key);
		if (s == GLFW_PRESS && !prev[key]) {
				item.second(window);
				prev[key] = 1;
		} else if (s == GLFW_RELEASE) {
				prev[key] = 0;
		}
	}
}

void KeysControls::printControls()
{
  std::cout << std::endl << "---------   KEYS   -----------" << std::endl << std::endl;

  std::cout << "Select next object - left" << std::endl;
  std::cout << "Select prev object - right" << std::endl;

  std::cout << std::endl;

  std::cout << "Remove behaviours from selection object - 0" << std::endl;
  std::cout << "Spin - 1" << std::endl;
  std::cout << "Translation - 2" << std::endl;

  std::cout << std::endl;

  std::cout << "X increase - U" << std::endl;
  std::cout << "X increase - J" << std::endl;

  std::cout << std::endl;

  std::cout << "Y increase - I" << std::endl;
  std::cout << "Y increase - K" << std::endl;

  std::cout << std::endl;

  std::cout << "Z increase - O" << std::endl;
  std::cout << "Z increase - L" << std::endl;

  std::cout << std::endl;

  std::cout << "flat/smooth shading - C" << std::endl;
  std::cout << "texture/colors - T" << std::endl;
  std::cout << "grayscale - G increase" << std::endl;

  std::cout << std::endl << "---------   KEYS   -----------" << std::endl << std::endl;
}
