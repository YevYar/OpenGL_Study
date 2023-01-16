#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <memory>
#include <vector>

#include "helpers/macros.h"
#include "shaderProgram.h"
#include "vertexArray.h"

namespace renderer
{
	/**
	 * \brief SceneObject is a base abstract class for all classes, which represents something, what can be rendered.
	 */
	class SceneObject
	{
		public:
			SceneObject() = default;
			SceneObject(std::shared_ptr<vertex::VertexArray> vao,
				std::shared_ptr<shader::ShaderProgram> shaderProgram);
			virtual ~SceneObject() = default;

			DEFAULT_COPYABLE_MOVABLE(SceneObject)

			/**
			 * \brief Draws something on the scene.
			 */
			virtual void draw() = 0;

		protected:
			std::shared_ptr<vertex::VertexArray> m_vao = nullptr;
			std::shared_ptr<shader::ShaderProgram> m_shaderProgram = nullptr;

	};
}

#endif
