#include "vertexBuffer.h"

#include <glad/glad.h>

#include "debugHelpers.h"

using namespace vertex;

VertexBuffer::VertexBuffer(const VertexData& data, DataUsage usage) :
	m_data {data}, m_usage {usage}
{
	GLCall(glGenBuffers(1, &m_rendererId));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
	GLCall(glBufferData(GL_ARRAY_BUFFER, data.size, data.data, static_cast<int>(usage)));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererId));
}

void VertexBuffer::bind() const noexcept
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
}

void VertexBuffer::unbind() const noexcept
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::setData(const VertexData& data)
{
	bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, data.size, data.data, static_cast<int>(m_usage)));
	m_data = data;
}

const VertexData& VertexBuffer::getData() const noexcept
{
	return m_data;
}
