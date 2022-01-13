#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& dimension) {

	this->position = position;
	this->rotation = rotation;
	this->dimension = dimension;

	parent = nullptr;
	pivotPoint = glm::vec3(0.0f);

	rotationLocal = glm::vec3(0.0f);
	dimensionLocal = glm::vec3(1.0f);

	modelMatrix = glm::mat4(1.0f);

	isModelMatrixOld = true;

}

Transform::Transform(const Transform& transform) {

	this->position = transform.position;
	this->rotation = transform.rotation;
	this->dimension = transform.dimension;

	parent = nullptr;
	pivotPoint = glm::vec3(0.0f);

	rotationLocal = glm::vec3(0.0f);
	dimensionLocal = glm::vec3(1.0f);

	modelMatrix = glm::mat4(1.0f);

	isModelMatrixOld = true;

}

Transform::~Transform() { }

void Transform::updateModelMatrix() {

	modelMatrix =

		glm::translate(glm::mat4(1.0f), glm::vec3(position)) *

		glm::scale(glm::mat4(1.0f), glm::vec3(dimension)) *

		glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *

		glm::rotate(glm::mat4(1.0f), glm::radians(rotationLocal.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rotationLocal.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rotationLocal.x), glm::vec3(1.0f, 0.0f, 0.0f)) *

		glm::scale(glm::mat4(1.0f), glm::vec3(dimensionLocal)) *

		glm::translate(glm::mat4(1.0f), glm::vec3(pivotPoint)) *

		glm::rotate(glm::mat4(1.0f), glm::radians(rotationLocal.x), glm::vec3(1.0f, 0.0f, 0.0f)) *

		glm::scale(glm::mat4(1.0f), glm::vec3(dimensionLocal)) *

		glm::translate(glm::mat4(1.0f), pivotPoint) *

		glm::translate(glm::mat4(1.0f), parent != nullptr ? *parent : glm::vec3(0.0f));

	isModelMatrixOld = false;

}

void Transform::setParent(glm::vec3* parent) {

	this->parent = parent;

	isModelMatrixOld = true;

}

void Transform::setPivotPoint(const glm::vec3& pivotPoint) {

	this->pivotPoint = pivotPoint;

	isModelMatrixOld = true;

}

void Transform::setPosition(const glm::vec3& position) {

	this->position = position;

	isModelMatrixOld = true;

}

void Transform::setRotation(const glm::vec3& rotation) {

	this->rotation = rotation;

	isModelMatrixOld = true;

}

void Transform::setRotationLocal(const glm::vec3& rotation) {

	rotationLocal = rotation;

	isModelMatrixOld = true;

}

void Transform::setDimension(const glm::vec3& dimension) {

	this->dimension = dimension;

	isModelMatrixOld = true;

}

void Transform::setDimensionLocal(const glm::vec3& dimension) {

	dimensionLocal = dimension;

	isModelMatrixOld = true;

}

void Transform::setTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& dimension) {

	this->position = position;
	this->rotation = rotation;
	this->dimension = dimension;

	isModelMatrixOld = true;

}

void Transform::setTransform(const Transform& transform) {

	this->position = transform.position;
	this->rotation = transform.rotation;
	this->dimension = transform.dimension;

	isModelMatrixOld = true;

}

void Transform::translate(const glm::vec3& translate) {

	position += translate;

	isModelMatrixOld = true;

}

void Transform::rotate(const glm::vec3& rotate) {

	rotation += rotate;

	isModelMatrixOld = true;

}

void Transform::scale(const glm::vec3& scale) {

	dimension += scale;

	isModelMatrixOld = true;

}

void Transform::transform(const glm::vec3& translate, const glm::vec3& rotate, const glm::vec3& scale) {

	position += translate;
	rotation += rotate;
	dimension += scale;

	isModelMatrixOld = true;

}

void Transform::rotateLocal(const glm::vec3& rotate) {

	rotationLocal += rotate;

	isModelMatrixOld = true;

}

void Transform::scaleLocal(const glm::vec3& scale) {

	dimensionLocal += scale;

	isModelMatrixOld = true;

}

void Transform::transformLocal(const glm::vec3& translate, const glm::vec3& rotate, const glm::vec3& scale) {

	position = translate;
	rotationLocal = rotate;
	dimensionLocal = scale;

	isModelMatrixOld = true;

}

glm::vec3 Transform::getPosition() {
	return position;
}

glm::vec3 Transform::getRotation() {
	return rotation;
}

glm::vec3 Transform::getRotationLocal() {
	return rotationLocal;
}

glm::vec3 Transform::getDimension() {
	return dimension;
}

glm::vec3 Transform::getDimensionLocal() {
	return dimensionLocal;
}

glm::vec3 Transform::getPivotPoint() {
	return pivotPoint;
}

const glm::mat4& const Transform::getModelMatrix() {

	if (isModelMatrixOld) { updateModelMatrix(); }

	return modelMatrix;

}

float* Transform::getModelMatrixData() {
	return glm::value_ptr(modelMatrix);
}