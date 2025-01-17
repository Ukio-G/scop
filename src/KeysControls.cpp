#include "IO/KeysControls.hpp"

KeysControls::KeysControls(Window &window) : window(&window) {
	eventSubscriber = Subscriber::create("KeysControlsSubscriber");
	EventChannel::getInstance().subscribe("NewKeyEvent", eventSubscriber);
	eventSubscriber->addActionToTopic("NewKeyEvent", "KeyPressed",[&](Subscriber::ActionArgument argument) {
		auto keyEvent = std::any_cast<std::pair<KeyType, ActionType>>(argument);
		actions[keyEvent.first] = keyEvent.second;
	});
}

void KeysControls::pollingKeysEvent() {
	for (auto &item: actions) {
		auto key = item.first;
		if (glfwGetKey(window->glfwWindow, key) == GLFW_PRESS)
			item.second(window);
	}
}
