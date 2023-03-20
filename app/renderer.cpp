#include "renderer.h"

#include "helpers/debugHelpers.h"
#include "multicoloredRectangle.h"
#include "openglLimits.h"

using namespace app::renderer;

struct Renderer::Impl
{
	// METHODS
	Impl()
	{
		openglCore::initOpenglLimits();
		m_ColoredRectangle = app::makeMulticoloredRectangle();
	}

	// DATA
	std::unique_ptr<app::MulticoloredRectangle> m_ColoredRectangle = nullptr;
	float m_currentK = 0.0f;
	float m_increment = 0.05f;

};

Renderer::Renderer() : m_impl{ std::make_unique<Impl>()}
{
}

Renderer::Renderer(std::unique_ptr<Impl> impl) : m_impl{ std::move(impl) }
{
}

void Renderer::draw()
{
	GLCall(glClearColor(0.1176f, 0.5647, 1.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));

	if (m_impl->m_currentK >= 1.0f)
		m_impl->m_increment = -0.05f;
	if (m_impl->m_currentK <= 0)
		m_impl->m_increment = 0.05f;

	m_impl->m_ColoredRectangle->setColorCoefficient(&m_impl->m_currentK);
	m_impl->m_ColoredRectangle->draw();

	m_impl->m_currentK += m_impl->m_increment;
}
