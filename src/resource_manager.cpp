#include "include/resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

std::map<unsigned int, byrone::Shader> byrone::ResourceManager::Shaders;
std::map<unsigned int, byrone::Texture2D> byrone::ResourceManager::Textures;

byrone::Shader byrone::ResourceManager::LoadShader(const char *vertexFile, const char *fragmentFile) {
	auto shader = compileShaderFiles(vertexFile, fragmentFile);

	Shaders[shader.id] = shader;

	return shader;
}

byrone::Shader byrone::ResourceManager::GetShader(unsigned int id) {
	return Shaders[id];
}

byrone::Texture2D byrone::ResourceManager::LoadTexture(const char *file, bool alpha, byrone::TextureType type) {
	auto texture = compileTextureFile(file, alpha, type);

	Textures[texture.id] = texture;

	return texture;
}

byrone::Texture2D byrone::ResourceManager::GetTexture(unsigned int id) {
	return Textures[id];
}

byrone::Model byrone::ResourceManager::LoadModel(const char *path, byrone::ModelType type) {
	auto *file = fopen(path, "r");

	if (file == nullptr) {
		std::cout << "Failed to open file:" << std::endl
				  << path << std::endl;
	}

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	switch (type) {
		case ModelType::OBJ:
			loadObjFile(vertices, uvs, normals, file, path);
			break;

		default:
			std::cout << "Unknown Model type" << std::endl;
			break;
	}

	byrone::Model model(vertices, uvs, normals);

	fclose(file);

	return model;
}

void byrone::ResourceManager::Clear() {
	for (const auto &iter: Shaders) {
		glDeleteProgram(iter.second.id);
	}

	for (const auto &iter: Textures) {
		glDeleteTextures(1, &iter.second.id);
	}
}

byrone::Shader byrone::ResourceManager::compileShaderFiles(const char *vertexFile, const char *fragmentFile) {
	std::string vertexCode;
	std::string fragmentCode;

	try {
		std::ifstream vertexShaderFile(vertexFile);
		std::ifstream fragmentShaderFile(fragmentFile);
		std::stringstream vertexStream, fragmentStream;

		if (!vertexShaderFile.is_open()) {
			std::cout << "Failed to read vertex shader:" << std::endl
					  << vertexFile << std::endl;
			return {};
		}

		if (!fragmentShaderFile.is_open()) {
			std::cout << "Failed to read fragment shader:" << std::endl
					  << fragmentFile << std::endl;
			return {};
		}

		vertexStream << vertexShaderFile.rdbuf();
		fragmentStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();
	}
	catch (std::exception &e) {
		std::cout << "Failed to read shader files:" << std::endl
				  << e.what() << std::endl;
	}

	Shader shader;
	shader.Compile(vertexCode.c_str(), fragmentCode.c_str());

	return shader;
}

byrone::Texture2D byrone::ResourceManager::compileTextureFile(const char *path, bool alpha, byrone::TextureType type) {
	Texture2D texture;

	if (alpha) {
		texture.internalFormat = GL_RGBA;
		texture.imageFormat = GL_RGBA;
	}

	auto *file = fopen(path, "rb");

	if (file == nullptr) {
		std::cout << "Failed to open file:" << std::endl
				  << path << std::endl;
		return texture;
	}

	switch (type) {
		case TextureType::BMP:
			loadBmpFile(texture, file, path);
			break;

		case TextureType::DDS:
			loadDdsFile(texture, file, path);
			break;

		default:
			std::cout << "Unknown texture type" << std::endl;
			break;
	}

	fclose(file);

	return texture;
}

void byrone::ResourceManager::loadBmpFile(byrone::Texture2D texture, FILE *file, const char *path) {
	unsigned char header[54];

	auto read = fread(header, 1, 54, file);

	if ((read != 54) || (header[0] != 'B') || (header[1] != 'M')) {
		std::cout << "Invalid BMP file:" << std::endl
				  << path << std::endl;

		return;
	}

	// width * height * 3
	unsigned int imageSize = *(int *) &(header[0x22]);
	GLsizei width = *(GLsizei *) &(header[0x12]);
	GLsizei height = *(GLsizei *) &(header[0x16]);

	if (imageSize == 0) {
		imageSize = width * height * 3;
	}

	auto *data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);

	texture.Generate(width, height, data);
}

void byrone::ResourceManager::loadDdsFile(byrone::Texture2D texture, FILE *file, const char *path) {
	char fileCode[4];
	fread(fileCode, 1, 4, file);

	if (strncmp(fileCode, "DDS ", 4) != 0) {
		std::cout << "Invalid DDS file:" << std::endl
				  << path << std::endl;

		return;
	}

	unsigned char header[124];

	fread(&header, 124, 1, file);

	GLsizei height = *(GLsizei *) &(header[8]);
	GLsizei width = *(GLsizei *) &(header[12]);
	unsigned int linearSize = *(unsigned int *) &(header[16]);
	unsigned int mipMapCount = *(unsigned int *) &(header[24]);
	unsigned int fourCC = *(unsigned int *) &(header[80]);

	// total size including all mipmap levels
	unsigned int bufferSize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	unsigned char *buffer = (unsigned char *) malloc(bufferSize * sizeof(unsigned char));

	fread(buffer, 1, bufferSize, file);

	texture.GenerateFromMipmap(width, height, fourCC, mipMapCount, buffer);

	free(buffer);
}

void byrone::ResourceManager::loadObjFile(std::vector<glm::vec3> &vertices,
										  std::vector<glm::vec2> &uvs,
										  std::vector<glm::vec3> &normals,
										  FILE *file,
										  const char *path) {
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	while (true) {
		// @todo BAD assumption
		char lineHeader[128];

		auto result = fscanf(file, "%s", lineHeader);

		if (result == EOF) {
			break;
		}

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		} else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		} else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		} else if (strcmp(lineHeader, "f") == 0) {
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
								 &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2],
								 &normalIndex[2]);

			if (matches != 9) {
				std::cout << "Parsing error for file: " << std::endl
						  << path << std::endl;
				return;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		} else {
			// Comment/other bogus line
			char temp[1000];
			fgets(temp, 1000, file);
		}
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		vertices.push_back(vertex);
		uvs.push_back(uv);
		normals.push_back(normal);
	}
}
