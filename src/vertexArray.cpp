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
            switch (attr.type)
            {
                case VertexAttrType::BYTE:
                case VertexAttrType::INT:
                case VertexAttrType::INT_2_10_10_10_REV:
                case VertexAttrType::SHORT:
                case VertexAttrType::UNSIGNED_BYTE:
                case VertexAttrType::UNSIGNED_INT:
                case VertexAttrType::UNSIGNED_INT_2_10_10_10_REV:
                case VertexAttrType::UNSIGNED_SHORT:
                case VertexAttrType::FIXED:
                case VertexAttrType::UNSIGNED_INT_10F_11F_11F_REV:
                    GLCall(glVertexAttribIPointer(attr.index, attr.count, helpers::toUType(attr.type),
                        stride, reinterpret_cast<void*>(attr.byteOffset)));
                    break;
                case VertexAttrType::FLOAT:
                case VertexAttrType::HALF_FLOAT:
                    GLCall(glVertexAttribPointer(attr.index, attr.count, helpers::toUType(attr.type),
                        attr.normalized ? GL_TRUE : GL_FALSE, stride, reinterpret_cast<void*>(attr.byteOffset)));
                    break;
                case VertexAttrType::DOUBLE:
                    GLCall(glVertexAttribLPointer(attr.index, attr.count, helpers::toUType(attr.type),
                        stride, reinterpret_cast<void*>(attr.byteOffset)));
                    break;
                default:
                    GLCall(glVertexAttribPointer(attr.index, attr.count, helpers::toUType(attr.type),
                        attr.normalized ? GL_TRUE : GL_FALSE, stride, reinterpret_cast<void*>(attr.byteOffset)));
            }
			
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
