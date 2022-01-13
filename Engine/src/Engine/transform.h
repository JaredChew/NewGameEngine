#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Transform {

private:

	glm::vec3* parent;
	glm::vec3 pivotPoint;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 dimension;

	glm::vec3 rotationLocal;
	glm::vec3 dimensionLocal;

	glm::mat4x4 orientation;
	glm::mat4x4 modelMatrix;

	bool isModelMatrixOld;

public:

	Transform(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& dimension = glm::vec3(1.0f)); //, const glm::vec3& pivotPoint = glm::vec3(0.0f)
	Transform(const Transform& transform);
	~Transform();

	void updateModelMatrix();

	void translate(const glm::vec3 &translate);
	void rotate(const glm::vec3 &rotate);
	void scale(const glm::vec3 &scale);
	void transform(const glm::vec3 &translate, const glm::vec3 &rotate, const glm::vec3 &scale);

	void rotateLocal(const glm::vec3 &rotate);
	void scaleLocal(const glm::vec3 &scale);
	void transformLocal(const glm::vec3 &translate, const glm::vec3 &rotate, const glm::vec3 &scale);

	void setPosition(const glm::vec3 &position);
	void setRotation(const glm::vec3 &rotation);
	void setRotationLocal(const glm::vec3 &rotation);
	void setDimension(const glm::vec3 &dimension);
	void setDimensionLocal(const glm::vec3 &dimension);
	void setParent(glm::vec3* parent);
	void setPivotPoint(const glm::vec3& pivotPoint);
	void setTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& dimension);
	void setTransform(const Transform& transform);

	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getRotationLocal();
	glm::vec3 getDimension();
	glm::vec3 getDimensionLocal();
	glm::vec3 getPivotPoint();

	const glm::mat4& const getModelMatrix();

	float *getModelMatrixData();

};