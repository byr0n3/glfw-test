#define STB_IMAGE_IMPLEMENTATION

#include <include/resource_manager.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <external/stb_image.h>

std::unordered_map<const char *, byrone::Shader> byrone::ResourceManager::Shaders;
std::unordered_map<const char *, byrone::Texture2D> byrone::ResourceManager::Textures;

byrone::Shader byrone::ResourceManager::LoadShader(const char *name, const char *vertexFile, const char *fragmentFile) {
	auto shader = compileShaderFiles(vertexFile, fragmentFile);

	Shaders[name] = shader;

	return shader;
}

byrone::Shader byrone::ResourceManager::GetShader(const char *name) {
	return Shaders[name];
}

byrone::Texture2D byrone::ResourceManager::LoadTexture(const char *name, const char *file, bool alpha) {
	auto texture = compileTextureFile(file, alpha);

	Textures[name] = texture;

	return texture;
}

byrone::Texture2D byrone::ResourceManager::GetTexture(const char *name) {
	return Textures[name];
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

byrone::Texture2D byrone::ResourceManager::compileTextureFile(const char *path, bool alpha) {
	Texture2D texture;

	if (alpha) {
		texture.internalFormat = GL_RGBA;
		texture.imageFormat = GL_RGBA;
	}

	int width, height, channels;
	unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

	if (stbi_failure_reason()) {
		std::cout << "Failed to read texture file:" << std::endl
				  << path << std::endl
				  << stbi_failure_reason() << std::endl;

		stbi_image_free(data);

		return texture;
	}

	texture.Generate(width, height, data);

	stbi_image_free(data);

	return texture;
}
