#pragma once

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Transform;

class Camera {

private:

	enum class ProjectionType { NONE, ORTHOGRAPHIC, PERSPECTIVE} projectionType;

	//glm::mat4 orientation;
	glm::mat4 projectionMatrix;

public:

	Transform* transform;

public:

	Camera(const glm::vec3 &position = glm::vec3(0.0f), const glm::vec3 &rotation = glm::vec3(0.0f), const glm::vec3 &dimension = glm::vec3(1.0f));
	~Camera();

	void setProjectionPerspective(const float& fov, const float& windowRatio, const float& near, const float& far);
	void setProjectionOrthographic(const float& left, const float& right, const float& bottom, const float& top, const float& near, const float& far);
	void setProjectionOrthographic(const float& windowWidth, const float& windowHeight, const float& near, const float& far, const float& fov);

	//glm::mat4& const getOrientation();
	glm::mat4& const getProjectionMatrix();

	const glm::mat4& const getViewMatrix();
	const glm::mat4& const getViewMatrixWithoutTranslate();

	glm::mat4 generateMvpMatrix(const glm::mat4& modelMatrix);

};