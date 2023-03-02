#include "buffer.h"

#include <glad/glad.h>

#include "exceptions.h"
#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"

using namespace vertex;

Buffer::Buffer(BufferTarget target, ArrayData data, BufferDataUsage usage,
	std::optional<VertexBufferLayout> bufferLayout) :
	m_target{ target }, m_data{ std::move(data) },
	m_usage{ usage }, m_layout{ std::move(bufferLayout) }
{
	genBuffer();
    loadData();
}

Buffer::Buffer(Buffer&& obj) noexcept :
	m_rendererId{ obj.m_rendererId }, m_target{ obj.m_target },
	m_data{ std::move(obj.m_data) }, m_usage{ obj.m_usage },
	m_layout{ std::move(obj.m_layout) }
{
	obj.m_rendererId = 0;
}

Buffer::~Buffer()
{
    deleteBuffer();
}

Buffer& Buffer::operator=(Buffer&& obj) noexcept
{
    deleteBuffer();

    m_rendererId = obj.m_rendererId;
	m_target = obj.m_target;
	m_usage = obj.m_usage;
	m_layout = std::move(obj.m_layout);
	m_data = std::move(obj.m_data);
	
	obj.m_rendererId = 0;
	
	return *this;
}

void Buffer::unbindTarget(BufferTarget target) noexcept
{
    GLCall(glBindBuffer(helpers::toUType(target), 0));
}

void Buffer::bind() const noexcept
{
	GLCall(glBindBuffer(helpers::toUType(m_target), m_rendererId));
}

void Buffer::unbind() const noexcept
{
	Buffer::unbindTarget(m_target);
}

void Buffer::setData(ArrayData data)
{
    m_data = std::move(data);
    loadData();
}

const ArrayData& Buffer::getData() const noexcept
{
	return m_data;
}

std::optional<VertexBufferLayout> vertex::Buffer::getLayout() const noexcept
{
	return m_layout;
}

void Buffer::genBuffer()
{
	GLCall(glGenBuffers(1, &m_rendererId));
	if (m_rendererId == 0)
	{
		throw exceptions::GLRecAcquisitionException("Buffer cannot be generated.");
	}
}

void Buffer::loadData() const noexcept
{
    // TODO
    bind();
    GLCall(glBufferData(helpers::toUType(m_target), m_data.size, m_data.pointer, helpers::toUType(m_usage)));
}

void Buffer::deleteBuffer() noexcept
{
    GLCall(glDeleteBuffers(1, &m_rendererId));
    m_rendererId = 0;
}
