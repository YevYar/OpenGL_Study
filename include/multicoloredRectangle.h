#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "sceneObject.h"
#include "uniforms.h"

namespace renderer
{
	/**
	 * \brief MulticoloredRectangle is a rectangle, the color of which can blinks by using setColorCoefficient().
	 */
	class MulticoloredRectangle : public SceneObject
	{
		public:
			MulticoloredRectangle() = delete;
		
			DEFAULT_COPYABLE_MOVABLE(MulticoloredRectangle)

			/**
			 * \brief Draws multicolored rectangle on the scene.
			 */
			void draw() override;

			/**
			 * \brief Sets the color coefficient.
			 */
			void setColorCoefficient(const float* k);

		private:
			MulticoloredRectangle(std::shared_ptr<vertex::VertexArray> vao,
				std::shared_ptr<shader::ShaderProgram> shaderProgram);

		private:
			shader::Uniform<float, 1>& m_colorCoefficient;

		friend std::unique_ptr<MulticoloredRectangle> makeMulticoloredRectangle();
	};

	/**
	 * \brief Creates new MulticoloredRectangle object.
	 * 
	 * \return std::unique_ptr on created MulticoloredRectangle.
	 * \throw exceptions::GLRecAcquisitionException(), see shader::makeShaderProgram().
	 */
	std::unique_ptr<MulticoloredRectangle> makeMulticoloredRectangle();

}

#endif
