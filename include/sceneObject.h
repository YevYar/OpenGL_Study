#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <memory>
#include <vector>

#include "helpers/macros.h"
#include "shaderProgram.h"
#include "textureUnit.h"
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

            /**
             * \brief Constructs new SceneObject using a vertex array object and 
             * a shader program.
             * 
             * \param vao - vertex array object.
             * \param shaderProgram - shader program.
             */
			SceneObject(std::shared_ptr<vertex::VertexArray> vao,
				std::shared_ptr<shader::ShaderProgram> shaderProgram);

            DEFAULT_COPYABLE_MOVABLE(SceneObject)

            /**
             * \brief Destructs the object.
             */
			virtual ~SceneObject() = default;			

			/**
			 * \brief Draws something on the scene.
			 */
			virtual void draw() = 0;

            virtual void setTexturesConfiguration(texture::TexturesConfiguration texturesConfiguration);

		protected:
            /**
             * \brief Vertex array object of the object.
             */
			std::shared_ptr<vertex::VertexArray> m_vao = nullptr;

            /**
             * \brief Shader program, which is used to render this object.
             */
			std::shared_ptr<shader::ShaderProgram> m_shaderProgram = nullptr;

            texture::TexturesConfiguration m_texturesConfiguration;

	};
}

#endif
