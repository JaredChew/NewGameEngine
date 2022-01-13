#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/vec2.hpp>

#define MOUSE_BUTTON_POLLING_RATE 500

#define MOUSE_BUTTON_LEFT GLFW_MOUSE_BUTTON_1
#define MOUSE_BUTTON_RIGHT GLFW_MOUSE_BUTTON_2
#define MOUSE_BUTTON_MIDDLE GLFW_MOUSE_BUTTON_3

class Window;

class Mouse {

private:

private:

	enum class ButtonState { CLEAR = -1, RELEASE = GLFW_RELEASE, PRESS = GLFW_PRESS, HOLD = GLFW_REPEAT };

	static glm::vec2 position;
	static glm::vec2 positionOffset;
	static glm::vec2 positionDirection;

	static double scrollValue;
	static double scrollOffset;

	static int scrollDirection;

	static int buttonPressStatus[3];
	static int buttonHoldRate[3];
	static bool buttonPressLock[3];

private:

	static void onMouseMove(GLFWwindow *wnd, double x, double y);
	static void onMouseWheelScroll(GLFWwindow *window, double xoffset, double yoffset);
	static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

public:

	Mouse(Window* window);
	~Mouse();

	void switchWindowCapture(Window* window);

	bool isPressed(const int &button);
	bool isPressedOnce(const int& button);
	bool isHold(const int &button);
	bool isReleased(const int& button);

	void postUpdate();
	void preUpdate();

	glm::vec2& const getPosition();
	glm::vec2& const getPositionOffset();
	glm::vec2& const getPositionDirection();

	double& const getScrollValue();
	double& const getOffsetScrol();

	int& const getScrollDirection();

};