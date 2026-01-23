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

  enum class Button : uint8_t { Left, Right, Middle, Count };
  enum class Phase  : uint8_t { Down, Pressed, Released, Count };

  using MouseButtonHandler = std::function<void(double, double)>;
  using MouseScrollHandler = std::function<void(double, double)>;
  using MouseMoveHandler = std::function<void(double, double, double, double)>;

	std::map<size_t, MouseButtonHandler> leftClickHandlers;
	std::map<size_t, MouseButtonHandler> rightClickHandlers;
	std::map<size_t, MouseScrollHandler> scrollHandlers;
	std::map<size_t, MouseMoveHandler> moveHandlers;

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
	  static bool prevLeftBtnPressed = false;
	  static bool prevRightBtnPressed = false;

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

		if (leftBtnPressed && !prevLeftBtnPressed) {
			for (auto &handler: leftClickHandlers)
				handler.second(pos[0], pos[1]);
		}

		if (rightBtnPressed && !prevRightBtnPressed) {
			for (auto &handler: rightClickHandlers)
				handler.second(pos[0], pos[1]);
		}

		for (auto &handler: moveHandlers)
			handler.second(pos[0], pos[1], delta_pos[0], delta_pos[1]);

	  prevLeftBtnPressed = leftBtnPressed;
	  prevRightBtnPressed = rightBtnPressed;
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
				moveHandlers[moveHandlers.size()] = std::any_cast<std::function<void(double, double, double, double)>>(action);
				break;
		}
	}

	void scrollCallback(GLFWwindow*, double xoffset, double yoffset) {
		for (auto &handler: scrollHandlers) {
			handler.second(xoffset, yoffset);
		}
	}
};


#endif
