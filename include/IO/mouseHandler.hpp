#ifndef MOUSEHANDLER_HPP
#define MOUSEHANDLER_HPP

#include <map>
#include <functional>
#include "Window.hpp"
#include "EventChannel.hpp"
#include "MouseEvents.hpp"

class MouseControls {
public:
	using EventType = EMouseEvent;
	using ActionType = std::any;

	std::map<size_t, std::function<void(double /* x */, double /* y */)>> leftClickHandlers;
	std::map<size_t, std::function<void(double /* x */, double /* y */)>> rightClickHandlers;
	std::map<size_t, std::function<void(double /* x offset */, double /* y offset */)>> scrollClickHandlers;
	std::map<size_t, std::function<void(double /* x */, double /* y */, double /* dx */, double /* dy */)>> moveClickHandlers;

	MouseControls(Window & window) : window (&window) {
		glfwWindow = window.getNativeWindowPtr();
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		int windowsSize[2];

		glfwGetWindowSize(glfwWindow, windowsSize, windowsSize + 1);
		glfwSetCursorPos(glfwWindow, (double)(windowsSize[0]/2.0),(double)(windowsSize[1]/2.0));

		eventSubscriber = Subscriber::create("MouseControlsSubscriber");
		EventChannel::getInstance().subscribe("NewMouseEvent", eventSubscriber);

		eventSubscriber->addActionToTopic("NewMouseEvent", "NewMouseEvent",[&](Subscriber::ActionArgument argument) {
			auto mouseEvent = std::any_cast<std::pair<EventType, ActionType>>(argument);
			assignNewEvent(mouseEvent.first, mouseEvent.second);
		});
	}

	void pollingMouseEvents() {
		bool leftBtnPressed = glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
		bool rightBtnPressed = glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

		double pos[2] {};
		double delta_pos[2] {};
		glfwGetCursorPos(glfwWindow, pos, pos + 1);
		static double prev_pos[2] {pos[0], pos[1]};

		for (int i = 0; i < 2; ++i) {
			delta_pos[i] = prev_pos[i] - pos[i];
			prev_pos[i] = pos[i];
		}

		if (leftBtnPressed) {
			for (auto &handler: leftClickHandlers)
				handler.second(pos[0], pos[1]);
		}

		if (rightBtnPressed) {
			for (auto &handler: rightClickHandlers)
				handler.second(pos[0], pos[1]);
		}

		for (auto &handler: moveClickHandlers)
			handler.second(pos[0], pos[1], delta_pos[0], delta_pos[1]);
	}
private:
	Window * window;
	GLFWwindow *glfwWindow;
	Subscriber::Ptr eventSubscriber;

	void assignNewEvent(EMouseEvent event, ActionType & action) {
		using namespace std::placeholders;
		switch (event) {
			case EME_CLICK_LEFT:
				leftClickHandlers[leftClickHandlers.size()] = std::any_cast<std::function<void(double, double)>>(action);
				break;
			case EME_CLICK_RIGHT:
				rightClickHandlers[rightClickHandlers.size()] = std::any_cast<std::function<void(double, double)>>(action);
				break;
			case EME_SCROLL: {
				std::function<void(GLFWwindow*, double, double )> callback = [this] (GLFWwindow* window_, double xoffset, double yoffset) {
					scrollCallback(window_, xoffset, yoffset);
				};
				using callbackType = void(*)(GLFWwindow*, double, double);
				glfwSetScrollCallback(glfwWindow, (callbackType)&callback);
			}
				break;
			case EME_MOVE:
				moveClickHandlers[moveClickHandlers.size()] = std::any_cast<std::function<void(double, double, double, double)>>(action);
				break;
		}
	}

	void scrollCallback(GLFWwindow*, double xoffset, double yoffset) {
		for (auto &handler: scrollClickHandlers) {
			handler.second(xoffset, yoffset);
		}
	}
};


#endif
