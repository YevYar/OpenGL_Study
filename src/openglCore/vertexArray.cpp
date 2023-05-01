#include "vertexArray.h"
#include "vertexArrayImpl.h"

#include "bufferImpl.h"
#include "exceptions.h"
#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"
#include "vertexBufferLayout.h"

namespace ogls::oglCore::vertex
{
VertexArray::VertexArray() : m_impl{std::make_unique<Impl>()}
{
    bind();
}

VertexArray::VertexArray(const VertexArray& obj) : m_impl{std::make_unique<Impl>()}
{
    for (const auto& buffer : obj.m_impl->buffers)
    {
        addBuffer(buffer);
    }
}

VertexArray::VertexArray(VertexArray&& obj) noexcept : m_impl{std::move(obj.m_impl)}
{
}

VertexArray::~VertexArray() = default;

void VertexArray::unbind() noexcept
{
    Impl::bindSpecificVao(0);
}

VertexArray& VertexArray::operator=(VertexArray&& obj) noexcept
{
    m_impl = std::move(obj.m_impl);

    return *this;
}

void VertexArray::addBuffer(std::shared_ptr<Buffer> buffer) noexcept
{
    const auto layout = buffer->getLayout();

    if (layout == std::nullopt)
    {
        auto boundVao = helpers::getOpenGLIntegerValue(GL_VERTEX_ARRAY_BINDING);
        if (boundVao == m_impl->rendererId)
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
        OGLS_GLCall(glVertexArrayVertexBuffer(m_impl->rendererId, 0, buffer->m_impl->rendererId, 0, stride));

        for (const auto& attr : layout->getAttributes())
        {
            using namespace helpers;


            enableAttribute(attr.index);
            OGLS_GLCall(glVertexArrayAttribBinding(m_impl->rendererId, attr.index, 0));

            switch (attr.type)
            {
                case VertexAttrType::Byte:
                    [[fallthrough]];
                case VertexAttrType::Fixed:
                    [[fallthrough]];
                case VertexAttrType::Int:
                    [[fallthrough]];
                case VertexAttrType::Int2101010Rev:
                    [[fallthrough]];
                case VertexAttrType::Short:
                    [[fallthrough]];
                case VertexAttrType::UnsignedByte:
                    [[fallthrough]];
                case VertexAttrType::UnsignedInt:
                    [[fallthrough]];
                case VertexAttrType::UnsignedInt10f11f11fRev:
                    [[fallthrough]];
                case VertexAttrType::UnsignedInt2101010Rev:
                    [[fallthrough]];
                case VertexAttrType::UnsignedShort:
                    OGLS_GLCall(glVertexArrayAttribIFormat(m_impl->rendererId, attr.index, attr.count,
                                                           toUType(attr.type), attr.byteOffset));
                    break;
                case VertexAttrType::Float:
                    [[fallthrough]];
                case VertexAttrType::HalfFloat:
                    OGLS_GLCall(glVertexArrayAttribFormat(m_impl->rendererId, attr.index, attr.count,
                                                          toUType(attr.type), attr.normalized, attr.byteOffset));
                    break;
                case VertexAttrType::Double:
                    OGLS_GLCall(glVertexArrayAttribLFormat(m_impl->rendererId, attr.index, attr.count,
                                                           toUType(attr.type), attr.byteOffset));
                    break;
                default:
                {
                    OGLS_ASSERT(false);
                    OGLS_GLCall(glVertexArrayAttribFormat(m_impl->rendererId, attr.index, attr.count,
                                                          toUType(attr.type), attr.normalized, attr.byteOffset));
                }
            }
        }
    }

    m_impl->buffers.push_back(std::move(buffer));
}

void VertexArray::bind() const noexcept
{
    Impl::bindSpecificVao(m_impl->rendererId);
}

void VertexArray::disableAttribute(unsigned int index) const noexcept
{
    OGLS_GLCall(glDisableVertexArrayAttrib(m_impl->rendererId, index));
}

void VertexArray::enableAttribute(unsigned int index) const noexcept
{
    OGLS_GLCall(glEnableVertexArrayAttrib(m_impl->rendererId, index));
}

const std::vector<std::shared_ptr<Buffer>>& VertexArray::getBuffers() const noexcept
{
    return m_impl->buffers;
}

VertexArray* VertexArray::clone() const
{
    return new VertexArray{*this};
}

//------ IMPLEMENTATION

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
    OGLS_GLCall(glBindVertexArray(vaoId));
}

void VertexArray::Impl::deleteVertexArray()
{
    OGLS_GLCall(glDeleteVertexArrays(1, &rendererId));
    rendererId = {0};
}

void VertexArray::Impl::genVertexArray()
{
    OGLS_GLCall(glCreateVertexArrays(1, &rendererId));
    if (rendererId == 0)
    {
        throw exceptions::GLRecAcquisitionException{"Vertex array cannot be generated."};
    }
}

}  // namespace ogls::oglCore::vertex
