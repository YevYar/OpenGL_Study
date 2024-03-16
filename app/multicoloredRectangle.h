#ifndef APP_MULTICOLORED_RECTANGLE_H
#define APP_MULTICOLORED_RECTANGLE_H

#include "renderer/sceneObject.h"

/**
 * \namespace app
 * \brief app namespace contains types and functions of demo application.
 */
namespace app
{
/**
 * \brief MulticoloredRectangle is a rectangle, the color of which can blinks by using setColorCoefficient().
 */
class MulticoloredRectangle : public ogls::renderer::SceneObject
{
    public:
        OGLS_DEFAULT_COPYABLE_MOVABLE(MulticoloredRectangle)
        MulticoloredRectangle() = delete;

        /**
         * \brief Sets the color coefficient.
         *
         * \param k - a number in the range [0, 1].
         * \throw std::out_of_range.
         */
        void setColorCoefficient(float k);

        /**
         * \brief Renders multicolored rectangle on the scene.
         */
        void render() override;

    private:
        /**
         * \brief Constructs an object with specified vertex array object and shader program.
         *
         * \param vao           - a vertex array object, which contains necessary vertex configuration.
         * \param shaderProgram - a shader program, which is used for rendering of the rectangle.
         */
        MulticoloredRectangle(std::shared_ptr<ogls::oglCore::vertex::VertexArray>   vao,
                              std::shared_ptr<ogls::oglCore::shader::ShaderProgram> shaderProgram);

    private:
        /**
         * \brief Coefficient, which is used to change the color while blinking.
         */
        ogls::oglCore::shader::VectorUniform<float, 1>& m_colorCoefficient;
        /**
         * \brief Counter to count a number of rendering iterations.
         */
        int                                             m_counter = {0};


        friend std::unique_ptr<MulticoloredRectangle> makeMulticoloredRectangle();

};  // class MulticoloredRectangle

/**
 * \brief Creates new MulticoloredRectangle object.
 *
 * \return std::unique_ptr on created MulticoloredRectangle.
 * \throw ogls::exceptions::GLRecAcquisitionException(), see ogls::oglCore::shader::makeShaderProgram().
 */
std::unique_ptr<MulticoloredRectangle> makeMulticoloredRectangle();

}  // namespace app

#endif
