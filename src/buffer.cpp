#include "buffer.h"

#include <glad/glad.h>

#include "exceptions.h"
#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"

using namespace vertex;

namespace
{
	BufferBindingTarget getTargetAssociatedGetParameter(BufferTarget target) noexcept;
}

Buffer::Buffer(BufferTarget target, ArrayData data, BufferDataUsage usage,
	std::optional<VertexBufferLayout> bufferLayout) :
	m_target{ target }, m_data{ std::move(data) },
	m_usage{ usage }, m_layout{ std::move(bufferLayout) }
{
	genBuffer();
    setData(std::move(m_data));
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
	Buffer::bindToTarget(m_target, m_rendererId);
}

void Buffer::unbind() const noexcept
{
	Buffer::unbindTarget(m_target);
}

void Buffer::setData(ArrayData data)
{
	if (!checkAndGenerateNewStorage(data))
	{
		GLCall(glNamedBufferSubData(m_rendererId, 0, m_data.size, m_data.pointer));
		m_data = std::move(data);
	}    
}

const ArrayData& Buffer::getData() const noexcept
{
	return m_data;
}

std::optional<VertexBufferLayout> vertex::Buffer::getLayout() const noexcept
{
	return m_layout;
}

void vertex::Buffer::bindToTarget(BufferTarget target, GLuint bufferId) noexcept
{
	GLCall(glBindBuffer(helpers::toUType(target), bufferId));
}

void Buffer::genBuffer()
{
	GLCall(glCreateBuffers(1, &m_rendererId));
	if (m_rendererId == 0)
	{
		throw exceptions::GLRecAcquisitionException("Buffer cannot be generated.");
	}
}

bool Buffer::checkAndGenerateNewStorage(const ArrayData& data) noexcept
{
	if (m_data.size != data.size)
	{
		GLCall(glNamedBufferData(m_rendererId, data.size, data.pointer, helpers::toUType(m_usage)));
		m_data = data;
		return true;
	}
	return false;
}

void vertex::Buffer::bindForAMomentAndExecute(const std::function<void()>& funcToExecute)
{
	using namespace helpers;

	GLuint boundBuffer = getOpenGLIntegerValue(toUType(getTargetAssociatedGetParameter(m_target)));
	bind();

	funcToExecute();

	bindToTarget(m_target, boundBuffer);
}

void Buffer::deleteBuffer() noexcept
{

    GLCall(glDeleteBuffers(1, &m_rendererId));
    m_rendererId = 0;
}

namespace
{
	BufferBindingTarget getTargetAssociatedGetParameter(BufferTarget target) noexcept
	{
		switch (target)
		{
			case BufferTarget::ARRAY_BUFFER:
				return BufferBindingTarget::ARRAY_BUFFER_BINDING;
			case BufferTarget::ATOMIC_COUNTER_BUFFER:
				return BufferBindingTarget::ATOMIC_COUNTER_BUFFER_BINDING;
			case BufferTarget::COPY_READ_BUFFER:
				return BufferBindingTarget::COPY_READ_BUFFER_BINDING;
			case BufferTarget::COPY_WRITE_BUFFER:
				return BufferBindingTarget::COPY_WRITE_BUFFER_BINDING;
			case BufferTarget::DISPATCH_INDIRECT_BUFFER:
				return BufferBindingTarget::DISPATCH_INDIRECT_BUFFER_BINDING;
			case BufferTarget::DRAW_INDIRECT_BUFFER:
				return BufferBindingTarget::DRAW_INDIRECT_BUFFER_BINDING;
			case BufferTarget::ELEMENT_ARRAY_BUFFER:
				return BufferBindingTarget::ELEMENT_ARRAY_BUFFER_BINDING;
			case BufferTarget::PIXEL_PACK_BUFFER:
				return BufferBindingTarget::PIXEL_PACK_BUFFER_BINDING;
			case BufferTarget::PIXEL_UNPACK_BUFFER:
				return BufferBindingTarget::PIXEL_UNPACK_BUFFER_BINDING;
			case BufferTarget::QUERY_BUFFER:
				return BufferBindingTarget::QUERY_BUFFER_BINDING;
			case BufferTarget::SHADER_STORAGE_BUFFER:
				return BufferBindingTarget::SHADER_STORAGE_BUFFER_BINDING;
			case BufferTarget::TEXTURE_BUFFER:
				return BufferBindingTarget::TEXTURE_BUFFER_BINDING;
			case BufferTarget::TRANSFORM_FEEDBACK_BUFFER:
				return BufferBindingTarget::TRANSFORM_FEEDBACK_BUFFER_BINDING;
			case BufferTarget::UNIFORM_BUFFER:
				return BufferBindingTarget::UNIFORM_BUFFER_BINDING;
		}
	}
}
