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
