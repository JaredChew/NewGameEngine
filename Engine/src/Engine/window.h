#pragma once

#include <GLFW/glfw3.h>

#include <chrono>
#include <string>

#define MINIMUM_SLEEP_TIME 10000000.0

#define DYNAMIC_WINDOW_TITLE_SIZE 80

class Window {

private:

	GLFWwindow *window;
	GLFWwindow *share;

	GLFWmonitor *monitor;

	static int width;
	static int height;

	static float windowRatio;

	std::string title;
	char dynamicTitle[DYNAMIC_WINDOW_TITLE_SIZE];

	bool vsync;
	bool displayFps;
	//bool usingDynamicTitle;

	double fpsLimit;

	long double totalFrames;
	long double targetDeltaTime;

	std::chrono::steady_clock::time_point tick;
	std::chrono::steady_clock::time_point tock;

private:

	void limitFps(long double& deltaTime);
	void fpsCounter(const long double& deltaTime);

	static void onWindowResized(GLFWwindow *window, int width, int height);

public:

	static bool initWindowLibrary(void(*error_callback)(int, const char*));

	Window(const int &width, const int &height, const std::string &title, GLFWmonitor *const monitor, GLFWwindow *const share);
	~Window();

	void update(const long double& deltaTime);

	void setVsync(const bool &onOff);
	void setFpsLimit(const double &fpsLimit);
	void setDisplayFps(const bool &displayFps);
	//void setDynamicTitle(const bool& dynamicTitle);

	bool isDestroyed();

	int& const getWidth();
	int& const getHeight();

	void makeWindowCurrent();

	float& const getWindowRatio();

	GLFWwindow *getWindow() const;

};