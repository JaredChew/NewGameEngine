#include "camera.h"

#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &dimension) {

	transform = new Transform(position, rotation, dimension);

	projectionMatrix = glm::mat4(1.0f);
	//orientation = glm::mat4(1.0f);

	projectionType = ProjectionType::NONE;
}

Camera::~Camera() { }

void Camera::setProjectionPerspective(const float& fov, const float& windowRatio, const float& near, const float& far) {

	projectionMatrix = glm::perspective<float>(glm::radians(fov), windowRatio, near, far);
	projectionType = ProjectionType::PERSPECTIVE;

}

void Camera::setProjectionOrthographic(const float& left, const float& right, const float& bottom, const float& top, const float& near, const float& far) {

	projectionMatrix = glm::ortho<float>(left, right, bottom, top, near, far);
	projectionType = ProjectionType::ORTHOGRAPHIC;

}

void Camera::setProjectionOrthographic(const float& windowWidth, const float& windowHeight, const float& near, const float& far, const float& fov) {

	float leftRight = 0.0f, topDown = 0.0f;

	GLfloat fovRadian = glm::radians(fov);

	if (windowHeight < windowWidth) {
		leftRight = 0.1f * fovRadian;
		topDown = 0.1f * windowHeight / windowWidth * fovRadian;
	}
	else if (windowHeight > windowWidth) {
		leftRight = 0.1f * windowWidth / windowHeight * fovRadian;
		topDown = 0.1f * fovRadian;
	}
	else {
		leftRight = 0.1f * fovRadian;
		topDown = 0.1f * fovRadian;
	}

	setProjectionOrthographic(-leftRight, leftRight, -topDown, topDown, near, far);

}

//glm::mat4& const Camera::getOrientation() { return orientation; }

const glm::mat4& const Camera::getViewMatrix() {

	return  transform->getModelMatrix(); //glm::inverse(viewMatrix)

}

const glm::mat4& const Camera::getViewMatrixWithoutTranslate() {

	return glm::mat4(

		glm::rotate(glm::mat4(1.0f), glm::radians(transform->getRotationLocal().x), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(transform->getRotationLocal().y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(transform->getRotationLocal().z), glm::vec3(0.0f, 0.0f, 1.0f))

	);

}

glm::mat4& const Camera::getProjectionMatrix() {
	return projectionMatrix;
}

glm::mat4 Camera::generateMvpMatrix(const glm::mat4& modelMatrix) {

	return getProjectionMatrix() * getViewMatrix() * modelMatrix;

}
