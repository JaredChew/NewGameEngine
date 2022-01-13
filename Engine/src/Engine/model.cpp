#include "model.h"

#include "Mesh.h"
#include "texture.h"

#include <fstream>

using Json = nlohmann::json;

Model::Model(const char* path) {

	this->path = path;

	std::ifstream sourceFile(path);

	std::string modelData;
	modelData.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());

	json.parse(modelData);

	rawVertices = readVerticesData(json, path);

	initModelMeshes();

}

Model::~Model() { }

std::vector<glm::vec2> Model::floatsToVec2(const std::vector<float>& floats) {

	std::vector<glm::vec2> vectors;

	for (int i = 0; i < floats.size(); i += 2) {

		vectors.push_back(glm::vec2(floats[i], floats[i + 1]));

	}

	return vectors;

}

std::vector<glm::vec3> Model::floatsToVec3(const std::vector<float>& floats) {

	std::vector<glm::vec3> vectors;

	for (int i = 0; i < floats.size(); i += 3) {

		vectors.push_back(glm::vec3(floats[i], floats[i + 1], floats[i + 2]));

	}

	return vectors;

}

std::vector<glm::vec4> Model::floatsToVec4(const std::vector<float>& floats) {

	std::vector<glm::vec4> vectors;

	for (int i = 0; i < floats.size(); i += 4) {

		vectors.push_back(glm::vec4(floats[i], floats[i + 1], floats[i + 2], floats[i + 3]));

	}

	return vectors;

}

glm::vec3 extractMeshTranslation(Json& node) {

	if (node.find("translation") == node.end()) { return glm::vec3(0.0f, 0.0f, 0.0f); }

	float translationValues[3];

	for (unsigned int i = 0; i < node["translation"].size(); ++i) {

		translationValues[i] = (node["translation"][i]);

	}

	return glm::make_vec3(translationValues);

}

glm::quat extractMeshRotation(Json& node) {

	if (node.find("rotation") == node.end()) { return glm::quat(0.0f, 0.0f, 0.0f, 0.0f); }

	float rotationValues[4] = {

		node["Rotation"][3],
		node["Rotation"][0],
		node["Rotation"][1],
		node["Rotation"][2],

	};

	return glm::make_quat(rotationValues);

}

glm::vec3 extractMeshScale(Json& node) {

	if (node.find("scale") == node.end()) { return glm::vec3(1.0f, 1.0f, 1.0f); }

	float scaleValues[3];

	for (unsigned int i = 0; i < node["scale"].size(); ++i) {

		scaleValues[i] = (node["scale"][i]);

	}

	return glm::make_vec3(scaleValues);

}

glm::mat4 extractMeshMatrix(Json& node) {

	glm::mat4 matrixNode = glm::mat4(1.0f);

	if (node.find("matrix") == node.end()) { return glm::mat4(1.0f); }

	float matrixValues[16];

	for (unsigned int i = 0; i < node["matrix"].size(); ++i) {

		matrixValues[i] = (node["matrix"][i]);

	}

	return glm::make_mat4(matrixValues);

}

std::vector<float> Model::extractMeshVertices(const nlohmann::json& accessor) {

	std::vector<float> floatVec;

	unsigned int bufferViewIndex = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accessorByteOffset = accessor.value("byteOffset", 0);

	std::string type = accessor["type"];

	Json bufferView = json["bufferViews"][bufferViewIndex];

	unsigned int byteOffset = bufferView["byteOffset"];

	unsigned int numPerVertex;

	if (type == "SCALAR") { numPerVertex = 1; } //int
	else if (type == "VEC2") { numPerVertex = 2; }
	else if (type == "VEC3") { numPerVertex = 3; }
	else if (type == "VEC4") { numPerVertex = 4; }
	else { throw std::invalid_argument("Type is invalid"); }

	unsigned int beginingOfData = byteOffset + accessorByteOffset;
	unsigned int lengthOfData = count * 4 * numPerVertex;

	for (unsigned int i = beginingOfData; i < beginingOfData + lengthOfData; ) {

		unsigned char bytes[] = { rawVertices[i++], rawVertices[i++], rawVertices[i++], rawVertices[i++] };

		float value;
		std::memcpy(&value, bytes, sizeof(float));

		floatVec.push_back(value);

	}

	return floatVec;

}

std::vector<GLuint> Model::extractMeshIndices(const nlohmann::json& accessor) {

	std::vector<GLuint> indices;

	unsigned int bufferViewIndex = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accessorByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	Json bufferView = json["bufferViews"][bufferViewIndex];

	unsigned int byteOffset = bufferView["byteOffset"];
	unsigned int beginingOfData = byteOffset + accessorByteOffset;

	switch (componentType) {

		case 5125: // uINT

			for (unsigned int i = beginingOfData; i < byteOffset + accessorByteOffset + count * 4; i += 4 ) {

				unsigned char bytes[] = { rawVertices[i], rawVertices[i+1], rawVertices[i+2], rawVertices[i+3] };

				unsigned int value;
				std::memcpy(&value, bytes, sizeof(unsigned int));

				indices.push_back((GLuint)value);

			}

			break;

		case 5123: // uShort

			for (unsigned int i = beginingOfData; i < byteOffset + accessorByteOffset + count * 2; i += 2 ) {

				unsigned char bytes[] = { rawVertices[i], rawVertices[i+1] };

				unsigned short value;
				std::memcpy(&value, bytes, sizeof(unsigned short));

				indices.push_back((GLuint)value);

			}

			break;

		case 5122: // short

			for (unsigned int i = beginingOfData; i < byteOffset + accessorByteOffset + count * 2; i += 2 ) {

				unsigned char bytes[] = { rawVertices[i], rawVertices[i+1] };

				short value;
				std::memcpy(&value, bytes, sizeof(short));

				indices.push_back((GLuint)value);

			}

			break;

	}

	return indices;

}

bool Model::configureTextureWithLoaded(const std::string& textPath, const int& mesh) {

	for (int i = 0; i < textureName.size(); ++i) {

		if (textPath == textureName[i]) {

			textureOrder.emplace(mesh, i);

			return true;

		}

	}

	return false;

}

void Model::loadTexture(const std::string& textPath, const std::string& fileDirectory) {

	if (textPath.find("baseColor") != std::string::npos) { //diffuse

		texture.push_back(Texture((fileDirectory + textPath).c_str(), Texture::TextureType::DIFFUSE));

	}
	else if (textPath.find("metallicRoughness") != std::string::npos) { //specular

		texture.push_back(Texture((fileDirectory + textPath).c_str(), Texture::TextureType::SPECULAR));

	}
	else if (textPath.find("normalTexture") != std::string::npos) { //normal
		texture.push_back(Texture((fileDirectory + textPath).c_str(), Texture::TextureType::NORMAL));
	}

}

void Model::assembleTextures() {

	std::string fileDirectory = path.substr(0, path.find_last_of('/') + 1);

	for (unsigned int i = 0; i < json["images"].size(); ++i) {

		std::string textPath = json["images"][i]["uri"];

		if (configureTextureWithLoaded(textPath, mesh.size())) { continue; }

		loadTexture(textPath, fileDirectory);

		textureOrder.emplace(mesh.size(), textureName.size());
		textureName.push_back(textPath);

	}

}

std::vector<unsigned char> Model::readVerticesData(const nlohmann::json& node, const std::string& path) {

	std::string uri = node["buffers"][0]["uri"];

	std::string fileDirectory = path.substr(0, path.find_last_of('/') + 1);

	std::string rawData;
	std::ifstream fileURIdata((fileDirectory + uri).c_str());
	rawData.assign((std::istreambuf_iterator< char >(fileURIdata)), std::istreambuf_iterator< char >());

	return std::vector<unsigned char>(rawData.begin(), rawData.end());;

}

void Model::initModelMeshes(glm::mat4 meshMatrix, const unsigned int& nextNode) {

	Json node = json["nodes"][nextNode];

	if (node.find("mesh") != node.end()) {

		unsigned int meshIndex = node["mesh"];

		loadMesh(meshIndex);
		loadMeshMapping(meshIndex);
		loadMeshTransform(node, meshMatrix);

		mesh.end()->init();

	}

	if (node.find("children") == node.end()) { return; }

	for (unsigned int i = 0; i < node["children"].size(); ++i) {

		initModelMeshes(meshMatrix, node["children"][i]);

	}

}

void Model::loadMesh(const unsigned int& meshIndex) {

	unsigned int posAcceessorIndex = json["meshes"][meshIndex]["primitives"][0]["sttributes"]["POSITION"];
	unsigned int normalAcceessorIndex = json["meshes"][meshIndex]["primitives"][0]["sttributes"]["NORMAL"];
	unsigned int indicesAcceessorIndex = json["meshes"][meshIndex]["primitives"][0]["sttributes"]["indices"];
	
	std::vector<float> positionRawVertices = extractMeshVertices(json["accessors"][posAcceessorIndex]);
	std::vector<float> normalRawVertices = extractMeshVertices(json["accessors"][normalAcceessorIndex]);

	std::vector<glm::vec3> positions = floatsToVec3(positionRawVertices);
	std::vector<glm::vec3> normals = floatsToVec3(normalRawVertices);
	std::vector<GLuint> indices = extractMeshIndices(json["accessors"][indicesAcceessorIndex]);

	mesh.emplace_back(positions, indices, normals);

}

void Model::loadMeshMapping(const unsigned int& meshIndex) {

	unsigned int colourAccessorIndex = json["meshes"][meshIndex]["primitives"][0]["sttributes"]["COLOR_0"];
	unsigned int texCoordAcceessorIndex = json["meshes"][meshIndex]["primitives"][0]["sttributes"]["TEXCOORD_0"];

	std::vector<float> colourRawVertices = extractMeshVertices(json["accessors"][colourAccessorIndex]);
	std::vector<float> uvRawVertices = extractMeshVertices(json["accessors"][texCoordAcceessorIndex]);

	std::vector<glm::vec4> colour = floatsToVec4(colourRawVertices);
	std::vector<glm::vec2> uv = floatsToVec2(uvRawVertices);

	mesh.end()->addToBuffer(colour);
	mesh.end()->addToBuffer(uv);

	assembleTextures();

}

void Model::loadMeshTransform(nlohmann::json& node, glm::mat4& meshMatrix) {

	glm::vec3 translate = extractMeshTranslation(node);
	glm::quat rotate = extractMeshRotation(node);
	glm::vec3 scale = extractMeshScale(node);
	glm::mat4 matrix = extractMeshMatrix(node);

	glm::mat4 translation = glm::translate(translation, translate);
	glm::mat4 rotation = glm::mat4_cast(rotate);
	glm::mat4 dimension = glm::scale(dimension, scale);

	meshMatrix *= matrix * translation * rotation * dimension;

	meshTranslations.push_back(translate);
	meshRotation.push_back(rotate);
	meshDimentions.push_back(scale);
	meshMatrices.push_back(meshMatrix);

}