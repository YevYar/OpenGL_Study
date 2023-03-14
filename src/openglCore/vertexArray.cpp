#include "vertexArray.h"
#include "vertexArrayImpl.h"

#include <glad/glad.h>

#include "buffer.h"
#include "bufferImpl.h"
#include "exceptions.h"
#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"
#include "vertexBufferLayout.h"

using namespace vertex;

VertexArray::VertexArray() : m_impl{ std::make_unique<Impl>() }
{
	bind();
}

VertexArray::VertexArray(const VertexArray& obj) : m_impl{ std::make_unique<Impl>() }
{
    for (const auto& buffer : obj.m_impl->m_buffers)
    {
        addBuffer(buffer);
    }
}

VertexArray::VertexArray(VertexArray&& obj) noexcept : m_impl{ std::move(obj.m_impl) }
{
}

VertexArray::~VertexArray() = default;

VertexArray& VertexArray::operator=(VertexArray&& obj) noexcept
{
    m_impl = std::move(obj.m_impl);

    return *this;
}

void VertexArray::unbind() noexcept
{
    Impl::bindSpecificVao(0);
}

VertexArray* VertexArray::clone() const
{
    return new VertexArray(*this);
}

void VertexArray::bind() const noexcept
{
    Impl::bindSpecificVao(m_impl->m_rendererId);
}

void VertexArray::addBuffer(std::shared_ptr<Buffer> buffer) noexcept
{
	const auto layout = buffer->getLayout();

    if (layout == std::nullopt)
    {
        GLuint boundVao = helpers::getOpenGLIntegerValue(GL_VERTEX_ARRAY_BINDING);
        if (boundVao == m_impl->m_rendererId)
        {
            buffer->bind();
        }
        else
        {
            bind();
            buffer->bind();
            Impl::bindSpecificVao(boundVao);
        }
    }
    else
	{
		const auto stride = layout->getStride();
        GLCall(glVertexArrayVertexBuffer(m_impl->m_rendererId, 0, buffer->m_impl->m_rendererId, 0, stride));

		for (const auto& attr : layout->getAttributes())
		{
            using namespace helpers;

            enableAttribute(attr.index);
            GLCall(glVertexArrayAttribBinding(m_impl->m_rendererId, attr.index, 0));

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
                    GLCall(glVertexArrayAttribIFormat(m_impl->m_rendererId, attr.index, attr.count, toUType(attr.type),
                        attr.byteOffset));
                    break;
                case VertexAttrType::FLOAT:
                case VertexAttrType::HALF_FLOAT:
                    GLCall(glVertexArrayAttribFormat(m_impl->m_rendererId, attr.index, attr.count, toUType(attr.type),
                        attr.normalized, attr.byteOffset));
                    break;
                case VertexAttrType::DOUBLE:
                    GLCall(glVertexArrayAttribLFormat(m_impl->m_rendererId, attr.index, attr.count, toUType(attr.type),
                        attr.byteOffset));
                    break;
                default:
                    GLCall(glVertexArrayAttribFormat(m_impl->m_rendererId, attr.index, attr.count, toUType(attr.type),
                        attr.normalized, attr.byteOffset));
            }
		}
	}	

    m_impl->m_buffers.push_back(std::move(buffer));
}

const std::vector<std::shared_ptr<Buffer>>& VertexArray::getBuffers() const noexcept
{
	return m_impl->m_buffers;
}

void VertexArray::enableAttribute(unsigned int index) const noexcept
{
    GLCall(glEnableVertexArrayAttrib(m_impl->m_rendererId, index));
}

void VertexArray::disableAttribute(unsigned int index) const noexcept
{
	GLCall(glDisableVertexArrayAttrib(m_impl->m_rendererId, index));
}


// IMPLEMENTATION

VertexArray::Impl::Impl()
{
    genVertexArray();
}

VertexArray::Impl::~Impl()
{
    deleteVertexArray();
}

void VertexArray::Impl::bindSpecificVao(GLuint vaoId) noexcept
{
    GLCall(glBindVertexArray(vaoId));
}

void VertexArray::Impl::genVertexArray()
{
    GLCall(glCreateVertexArrays(1, &m_rendererId));
    if (m_rendererId == 0)
    {
        throw exceptions::GLRecAcquisitionException("Vertex array cannot be generated.");
    }
}

void VertexArray::Impl::deleteVertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_rendererId));
    m_rendererId = 0;
}
