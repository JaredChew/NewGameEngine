#include "window.h"

#include "timer.h"
#include "logger.h"

#include <thread>

int Window::width;
int Window::height;

float Window::windowRatio;

bool Window::initWindowLibrary(void(*error_callback)(int, const char*)) {

	static bool initStatus = false;

	if (initStatus) { return true; }

	glfwSetErrorCallback(error_callback);

	//Initialize GLFW library
	if (!glfwInit()) { return false; }

	//Set window hints
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

	return true;

}

Window::Window(const int &width, const int &height, const std::string &title, GLFWmonitor *const monitor, GLFWwindow *const share) {

	window = glfwCreateWindow(width, height, title.c_str(), monitor, share);

	this->width = width;
	this->height = height;

	this->windowRatio = static_cast<float>(width) / static_cast<float>(height);

	this->title = title;

	this->monitor = monitor;
	this->share = share;

	vsync = false;
	displayFps = false;

	fpsLimit = 0.0;
	totalFrames = 0.0;

	// Hook window resize.
	glfwSetWindowSizeCallback(window, onWindowResized);

	if (!window) {
		
		glfwTerminate();
		
		delete this;
	
	}

}

Window::~Window() {

	glfwDestroyWindow(window);

	glfwTerminate();

}

void Window::limitFps(long double& deltaTime) {

	while (deltaTime < targetDeltaTime) {

		if (targetDeltaTime - deltaTime > MINIMUM_SLEEP_TIME) { //1000000.0

			std::chrono::duration<double, std::nano> deltaTargetCurrent(0.7 * (targetDeltaTime - deltaTime));
			//std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTargetCurrent));

		}

		tock = std::chrono::high_resolution_clock::now();

		deltaTime = std::chrono::duration<long double, std::nano>(tock - tick).count();

	}

	//printf("currentTime: %f | targetDeltaTime: %f", currentTime, targetDeltaTime);

}

void Window::fpsCounter(const long double& deltaTime) {

	totalFrames = 1000000000.0 / deltaTime;

	if (displayFps) {
		sprintf(dynamicTitle, "%s | FPS: %f", title.c_str(), totalFrames);
		glfwSetWindowTitle(window, dynamicTitle);
		//printf("\nFrames: %f\n\n", totalFrames);
	}

}

void Window::onWindowResized(GLFWwindow *window, int widthResize, int heightResize) {

	if (heightResize == 0) heightResize = 1;						// Prevent A Divide By Zero By making Height Equal One

	width = widthResize;
	height = heightResize;

	windowRatio = static_cast<float>(width) / static_cast<float>(height);

	glViewport(0, 0, width, height);					// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

}

void Window::update(const long double& deltaTime) {

	tick = std::chrono::high_resolution_clock::now();

	long double deltaTime_Local = deltaTime;

	if (fpsLimit > 0.0) { limitFps(deltaTime_Local); }

	fpsCounter(deltaTime_Local);

	//Swap front and back buffer
	glfwSwapBuffers(window);

	//Get window and input events
	glfwPollEvents();

}

void Window::setVsync(const bool &vsync) {

	this->vsync = vsync;

	glfwSwapInterval(vsync);

}

void Window::setFpsLimit(const double &fpsLimit) {

	this->fpsLimit = fpsLimit;

	targetDeltaTime = 1000000000.0 / fpsLimit;

}

void Window::setDisplayFps(const bool &displayFps) {
	this->displayFps = displayFps;
}

bool Window::isDestroyed() {
	return glfwWindowShouldClose(window);
}

int& const Window::getWidth() {
	return width;
}

int& const Window::getHeight() {
	return height;
}

void Window::makeWindowCurrent() {

	glfwMakeContextCurrent(window);

}

float& const Window::getWindowRatio() {
	return windowRatio;
}

GLFWwindow *Window::getWindow() const {
	return window;
}