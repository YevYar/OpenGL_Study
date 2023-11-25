#include "renderer.h"

#include "helpers/debugHelpers.h"
#include "multicoloredRectangle.h"
#include "openglLimits.h"

namespace app::renderer
{
class Renderer::Impl
{
    public:
        Impl()
        {
            ogls::oglCore::initOpenglLimits();
            coloredRectangle = makeMulticoloredRectangle();
        }

        virtual ~Impl() noexcept = default;

    public:
        std::unique_ptr<MulticoloredRectangle> coloredRectangle = nullptr;
        float                                  currentK         = {0.0};
        float                                  increment        = {0.05};

};  // Renderer::Impl

Renderer::Renderer() : m_impl{std::make_unique<Impl>()}
{
}

void Renderer::render()
{
    OGLS_GLCall(glClearColor(0.1176f, 0.5647, 1.0f, 1.0f));
    OGLS_GLCall(glClear(GL_COLOR_BUFFER_BIT));

    if (m_impl->currentK >= 1.0f)
    {
        m_impl->increment = -0.05f;
    }
    else if (m_impl->currentK <= 0)
    {
        m_impl->increment = 0.05f;
    }

    m_impl->coloredRectangle->setColorCoefficient(m_impl->currentK);
    m_impl->coloredRectangle->render();

    m_impl->currentK += m_impl->increment;
}

}  // namespace app::renderer
