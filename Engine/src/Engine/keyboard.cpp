#include "keyboard.h"

#include "window.h"

int Keyboard::keyPressStatus_set1[SET_1_KEYS];
int Keyboard::keyPressStatus_set2[SET_2_KEYS];

Keyboard::Keyboard(Window* window) : window(window) {

	returnStatus = false;

	for (int i = 0; i < SET_1_KEYS; ++i) { keyPressStatus_set1[i] = (int)KeyState::CLEAR; }

	for (int i = 0; i < SET_2_KEYS; ++i) { keyPressStatus_set2[i] = (int)KeyState::CLEAR; }

	glfwSetKeyCallback(window->getWindow(), key_callback);

}

Keyboard::~Keyboard() { }

void Keyboard::setKeyPressStatus(const char &key, const int &state) {

	if (key < SET_1_KEYS_END) { keyPressStatus_set1[key] = state; }
	else { keyPressStatus_set2[SET_2_KEYS_END - key] = state; }

}

void Keyboard::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_UNKNOWN) { return; }

	setKeyPressStatus(key, action);

}

bool Keyboard::isKeyValid(const char &key) {

	if (key < SET_1_KEYS_START) { return false; }
	if (key > SET_2_KEYS_END) { return false; }
	if (key > SET_1_KEYS_END &&key < SET_2_KEYS_START) { return false; }

	return true;

}

int& const Keyboard::getKeyPressStatus(const char &key) {

	if (key < SET_1_KEYS_END) { return keyPressStatus_set1[key]; }
	else { return keyPressStatus_set2[SET_2_KEYS_END - key]; }

}

bool Keyboard::isPressed(const char &key) {

	return glfwGetKey(window->getWindow(), key);

}

bool Keyboard::isPressedOnce(const char &key) {

	if (!isKeyValid(key)) { return false; }

	return getKeyPressStatus(key) == (int)KeyState::PRESS;

}

bool Keyboard::isReleased(const char &key) {

	if (!isKeyValid(key)) { return false; }

	return getKeyPressStatus(key) == (int)KeyState::RELEASE;

}

bool Keyboard::isHold(const char &key) {

	if (!isKeyValid(key)) { return false; }

	return keyPressStatus_set1[key] == (int)KeyState::HOLD;

}

void Keyboard::postUpdate() {

	for (int i = 0; i < SET_1_KEYS; ++i) {

		if (keyPressStatus_set1[i] == (int)KeyState::PRESS || keyPressStatus_set1[i] == (int)KeyState::RELEASE) { keyPressStatus_set1[i] = (int)KeyState::CLEAR; }

	}

	for (int i = 0; i < SET_2_KEYS; ++i) {

		if (keyPressStatus_set2[i] == (int)KeyState::PRESS || keyPressStatus_set2[i] == (int)KeyState::RELEASE) { keyPressStatus_set2[i] = (int)KeyState::CLEAR; }

	}

}

void Keyboard::switchWindowCapture(Window* window) {

	this->window = window;

	glfwSetKeyCallback(window->getWindow(), key_callback);

}