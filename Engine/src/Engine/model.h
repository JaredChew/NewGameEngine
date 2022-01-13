#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../json.hpp"
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

class Mesh;
class Texture;

//using Json = nlohmann::json;

class Model {

private:

	nlohmann::json json;

	std::string path;
	std::vector<unsigned char> rawVertices; //binary data

	std::vector<Mesh> mesh;

	std::vector<glm::vec3> meshTranslations;
	std::vector<glm::quat> meshRotation;
	std::vector<glm::vec3> meshDimentions;
	std::vector<glm::mat4> meshMatrices;

	std::vector<Texture> texture;
	std::multimap<int, int> textureOrder;
	std::vector<std::string> textureName;

private:

	std::vector<glm::vec2> floatsToVec2(const std::vector<float>& floats);
	std::vector<glm::vec3> floatsToVec3(const std::vector<float>& floats);
	std::vector<glm::vec4> floatsToVec4(const std::vector<float>& floats);

	std::vector<float> extractMeshVertices(const nlohmann::json& accessor);
	std::vector<GLuint> extractMeshIndices(const nlohmann::json& accessor);

	bool configureTextureWithLoaded(const std::string& textPath, const int& mesh);
	void loadTexture(const std::string& textPath, const std::string& fileDirectory);
	void assembleTextures();

	std::vector<unsigned char> readVerticesData(const nlohmann::json& node, const std::string& path);

	void initModelMeshes(glm::mat4 matrix = glm::mat4(1.0f), const unsigned int& nextNode = 0);
	void loadMesh(const unsigned int& meshIndex);
	void loadMeshMapping(const unsigned int& meshIndex);
	void loadMeshTransform(nlohmann::json& node, glm::mat4& meshMatrix);

public:

	Model(const char* path);
	~Model();



};