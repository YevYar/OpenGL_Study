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

    private:
        using ogls::renderer::SceneObject::SceneObject;

        /**
         * \brief Renders multicolored rectangle on the scene.
         *
         * \param renderer - the object, which performs a rendering operation.
         */
        void render(std::shared_ptr<ogls::renderer::Renderer> renderer) override;

    private:
        /**
         * \brief Coefficient, which is used to change the color while blinking.
         */
        ogls::renderer::Material::Variable m_colorCoefficient;
        /**
         * \brief Counter to count a number of rendering iterations.
         */
        int                                m_counter = {0};
        /**
         * \brief The texture of the rectangle.
         */
        ogls::renderer::Material::Texture  m_mainTexture;


        friend std::unique_ptr<MulticoloredRectangle> makeMulticoloredRectangle();

};  // class MulticoloredRectangle

/**
 * \brief Creates new MulticoloredRectangle object.
 *
 * \return std::unique_ptr on created MulticoloredRectangle.
 * TODO: think about the exceptions
 * \throw ogls::exceptions::GLRecAcquisitionException(), see ogls::oglCore::shader::makeShaderProgram().
 */
std::unique_ptr<MulticoloredRectangle> makeMulticoloredRectangle();

}  // namespace app

#endif
