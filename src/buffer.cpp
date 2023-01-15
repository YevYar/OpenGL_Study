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
}

Buffer::~Buffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererId));
}

Buffer::Buffer(const Buffer& obj) :
	m_target{ obj.m_target }, m_data{ obj.m_data },
	m_usage{ obj.m_usage }, m_layout{ obj.m_layout }
{
	genBuffer();
}

Buffer::Buffer(Buffer&& obj) noexcept :
	m_rendererId{ obj.m_rendererId }, m_target{ obj.m_target },
	m_data{ std::move(obj.m_data) }, m_usage{ obj.m_usage },
	m_layout{ std::move(obj.m_layout) }, m_isDataSet{ obj.m_isDataSet }
{
	obj.m_rendererId = 0;
	obj.m_isDataSet = true;
}

Buffer& Buffer::operator=(const Buffer& obj)
{
	m_target = obj.m_target;
	m_usage = obj.m_usage;
	m_layout = obj.m_layout;
	m_data = obj.m_data;
	m_isDataSet = false;

	return *this;
}

Buffer& Buffer::operator=(Buffer&& obj) noexcept
{
	m_target = obj.m_target;
	m_usage = obj.m_usage;
	m_layout = std::move(obj.m_layout);
	m_data = std::move(obj.m_data);
	m_isDataSet = false;

	obj.m_rendererId = 0;
	obj.m_isDataSet = true;

	return *this;
}

void Buffer::bind() const noexcept
{
	GLCall(glBindBuffer(helpers::toUType(m_target), m_rendererId));
	if (!m_isDataSet)
	{
		updateData();
	}
}

void Buffer::unbind() const noexcept
{
	GLCall(glBindBuffer(helpers::toUType(m_target), 0));
}

void Buffer::setData(ArrayData data)
{
	/* 
	 * Set m_isDataSet to true to avoid loading previously set data.
	 * For example, the following code will cause unnecessary loading data in OpenGL buffer.
	 * Buffer b2(otherBuffer); // data of buffer b2 will be loaded only after calling b2.bind()
	 * b2.setData(newData); // without m_isDataSet = true, bind() inside setData() will cause
	 * loading previously set data in OpenGL buffer and only after that newData will be loaded.
	 */
	m_isDataSet = true;
	bind();
	m_data = std::move(data);
	updateData();
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

void Buffer::updateData() const noexcept
{
	GLCall(glBufferData(helpers::toUType(m_target), m_data.size, m_data.pointer, helpers::toUType(m_usage)));
	m_isDataSet = true;
}
