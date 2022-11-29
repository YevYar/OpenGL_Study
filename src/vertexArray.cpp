#include "vertexArray.h"

#include <glad/glad.h>

#include "buffer.h"
#include "debugHelpers.h"
#include "helpers.h"
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

void VertexArray::bind() const noexcept
{
	GLCall(glBindVertexArray(m_rendererId));
}

void VertexArray::unbind() const noexcept
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::addBuffer(const Buffer& vbo, const VertexBufferLayout& layout) const noexcept
{
	bind();
	vbo.bind();

	const auto stride = layout.getStride();

	for (const auto& attr : layout.getAttributes())
	{
		GLCall(glVertexAttribPointer(attr.index, attr.count, helpers::toUType(attr.type),
			attr.normalized ? GL_TRUE : GL_FALSE, stride, reinterpret_cast<void*>(attr.byteOffset)));
		enableAttribute(attr.index);
	}	
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
