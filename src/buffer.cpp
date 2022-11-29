#include "buffer.h"

#include <glad/glad.h>

#include "debugHelpers.h"
#include "helpers.h"

using namespace vertex;

Buffer::Buffer(BufferTarget target, const ArrayData& data, BufferDataUsage usage) :
	m_target {target}, m_data {data}, m_usage {usage}
{
	GLCall(glGenBuffers(1, &m_rendererId));
	setData(data);
}

Buffer::~Buffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererId));
}

void Buffer::bind() const noexcept
{
	GLCall(glBindBuffer(helpers::toUType(m_target), m_rendererId));
}

void Buffer::unbind() const noexcept
{
	GLCall(glBindBuffer(helpers::toUType(m_target), 0));
}

void Buffer::setData(const ArrayData& data)
{
	bind();
	GLCall(glBufferData(helpers::toUType(m_target), data.size, data.pointer, helpers::toUType(m_usage)));
	m_data = data;
}

const ArrayData& Buffer::getData() const noexcept
{
	return m_data;
}
