#include "include/resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

std::map<unsigned int, byrone::Texture2D>    byrone::ResourceManager::Textures;
std::map<unsigned int, byrone::Shader>       byrone::ResourceManager::Shaders;

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
