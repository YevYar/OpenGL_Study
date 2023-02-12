#include "vertexArray.h"

#include <glad/glad.h>

#include "buffer.h"
#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"
#include "vertexBufferLayout.h"

using namespace vertex;

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_rendererId));
	bind();
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_rendererId));
}

void VertexArray::unbind() noexcept
{
    GLCall(glBindVertexArray(0));
}

void VertexArray::bind() const noexcept
{
	GLCall(glBindVertexArray(m_rendererId));
}

void VertexArray::addBuffer(std::shared_ptr<Buffer> buffer) noexcept
{
	bind();
	buffer->bind();

	const auto layout = buffer->getLayout();

	if (layout != std::nullopt)
	{
		const auto stride = layout->getStride();

		for (const auto& attr : layout->getAttributes())
		{
			GLCall(glVertexAttribPointer(attr.index, attr.count, helpers::toUType(attr.type),
				attr.normalized ? GL_TRUE : GL_FALSE, stride, reinterpret_cast<void*>(attr.byteOffset)));
			enableAttribute(attr.index);
		}
	}	

	m_buffers.push_back(std::move(buffer));
}

const std::vector<std::shared_ptr<Buffer>>& vertex::VertexArray::getBuffers() const noexcept
{
	return m_buffers;
}

void VertexArray::enableAttribute(unsigned int index) const noexcept
{
	bind();
	GLCall(glEnableVertexAttribArray(index));
}

void VertexArray::disableAttribute(unsigned int index) const noexcept
{
	bind();
	GLCall(glDisableVertexAttribArray(index));
}
