#ifndef BYRONE_SPRITE_RENDERER
#define BYRONE_SPRITE_RENDERER

#include <include/shader.h>
#include <include/texture2d.h>

namespace byrone {
	class SpriteRenderer {
	public:
		explicit SpriteRenderer(Shader &shader);

		~SpriteRenderer();

		void Draw(Texture2D &texture,
				  glm::vec2 position,
				  glm::vec2 size = glm::vec2(10.0f, 10.0f),
				  float rotation = 0.0f,
				  glm::vec3 color = glm::vec3(1.0f));

	private:
		Shader shader;
		unsigned int vao;

		void initialize();
	};
}

#endif
