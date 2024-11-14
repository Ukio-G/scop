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
	Window * window;

	KeysControls(Window & window);
	void pollingKeysEvent();
private:
	Subscriber::Ptr eventSubscriber;
};


#endif //KEYSCONTROLS_HPP
