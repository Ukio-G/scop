#ifndef KEYSCONTROLS_HPP
#define KEYSCONTROLS_HPP


#include <functional>
#include <map>
#include "Window.hpp"
#include "EventChannel.hpp"

class KeysControls {
public:
	using ActionType = std::function<void(Window * window)>;
	using KeyType = int;
	std::map<KeyType, ActionType> actions;
	std::map<KeyType, ActionType> actionsReleased;
	std::array<unsigned char, GLFW_KEY_LAST + 1> prev{};
	Window * window;

	KeysControls(Window & window);
	void pollingKeysEvent();
private:
	Subscriber::Ptr eventSubscriber;
};


#endif //KEYSCONTROLS_HPP
