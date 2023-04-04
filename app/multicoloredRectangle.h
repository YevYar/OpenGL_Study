#ifndef OPENGLSTUDY_APP_MULTICOLOREDRECTANGLE_H
#define OPENGLSTUDY_APP_MULTICOLOREDRECTANGLE_H

#include "sceneObject.h"
#include "uniforms.h"

namespace openglStudy::app
{
	/**
	 * \brief MulticoloredRectangle is a rectangle, the color of which can blinks by using setColorCoefficient().
	 */
	class MulticoloredRectangle : public openglStudy::renderer::SceneObject
	{
        // METHODS
		public:
            DEFAULT_COPYABLE_MOVABLE(MulticoloredRectangle);
			MulticoloredRectangle() = delete;        

            /**
             * \brief Sets the color coefficient.
             *
             * \param k - is a number in the range [0, 1].
             */
            void setColorCoefficient(float k);

			/**
			 * \brief Draws multicolored rectangle on the scene.
			 */
			void draw() override;

		private:
            /**
             * \brief Constructs an object with specified VAO and shader program.
             * 
             * \param vao - a vertex array object, which contains necessary vertex configuration.
             * \param shaderProgram - a shader program, which is used for rendering of the rectangle.
             */
			MulticoloredRectangle(std::shared_ptr<openglStudy::openglCore::vertex::VertexArray> vao,
				std::shared_ptr<openglStudy::openglCore::shader::ShaderProgram> shaderProgram);

        // DATA
		private:
            /**
             * \brief Coefficient, which is used to change the color while blinking.
             */
            openglStudy::openglCore::shader::Uniform<float, 1>& m_colorCoefficient;
            /**
             * \brief Counter to count a number of rendering iterations.
             */
            int m_counter = {0};

		friend std::unique_ptr<MulticoloredRectangle> makeMulticoloredRectangle();

	};

	/**
	 * \brief Creates new MulticoloredRectangle object.
	 * 
	 * \return std::unique_ptr on created MulticoloredRectangle.
	 * \throw exceptions::GLRecAcquisitionException(), see shader::makeShaderProgram().
	 */
	std::unique_ptr<MulticoloredRectangle> makeMulticoloredRectangle();

}  // namespace openglStudy::app

#endif
