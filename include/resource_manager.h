#ifndef BYRONE_RESOURCE_MANAGER
#define BYRONE_RESOURCE_MANAGER

#include <map>
#include <string>

#include "include/model.h"
#include "include/shader.h"
#include "include/texture2d.h"

namespace byrone {
	// manages resoures like shaders and textures
	class ResourceManager {
	public:
		// resources
		static std::map<unsigned int, Model> Models;
		static std::map<unsigned int, Shader> Shaders;
		static std::map<unsigned int, Texture2D> Textures;

		// loads and compile a shader from the given files.
		static Shader LoadShader(const char *vertexFile, const char *fragmentFile);

		// retrieves a stored shader
		static Shader GetShader(unsigned int id);

		// loads and compile a texture from file
		static Texture2D LoadTexture(const char *path, bool alpha, TextureType type);

		// retrieves a stored texture
		static Texture2D GetTexture(unsigned int id);

		static Model LoadModel(const char *path, ModelType type);

		static Model GetModel(unsigned int id);

		// deallocates all loaded resources
		static void Clear();

		~ResourceManager() {
			byrone::ResourceManager::Clear();
		}

	private:
		// load and compile a shader from the given files
		static Shader compileShaderFiles(const char *vertexFile, const char *fragmentFile);

		// loads a single texture from a file
		static Texture2D compileTextureFile(const char *file, bool alpha, TextureType type);

		static void loadBmpFile(Texture2D texture, FILE *file, const char *path);

		static void loadDdsFile(Texture2D texture, FILE *file, const char *path);

		static void loadObjFile(std::vector<glm::vec3> &vertices,
								std::vector<glm::vec2> &uvs,
								std::vector<glm::vec3> &normals,
								FILE *file,
								const char *path);
	};
}

#endif
