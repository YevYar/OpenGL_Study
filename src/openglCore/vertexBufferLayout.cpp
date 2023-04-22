#include "vertexBufferLayout.h"
#include "vertexBufferLayoutImpl.h"

#include <format>
#include <stdexcept>

#include "helpers/debugHelpers.h"
#include "openglLimits.h"

namespace ogls::oglCore::vertex
{
VertexBufferLayout::VertexBufferLayout() : m_impl{std::make_unique<Impl>()}
{
}

VertexBufferLayout::VertexBufferLayout(const VertexBufferLayout& obj) : m_impl{std::make_unique<Impl>(*obj.m_impl)}
{
}

VertexBufferLayout::~VertexBufferLayout() = default;

VertexBufferLayout& VertexBufferLayout::operator=(const VertexBufferLayout& obj)
{
    m_impl = std::make_unique<Impl>(*obj.m_impl);
    return *this;
}

void VertexBufferLayout::addVertexAttribute(const VertexAttribute& va)
{
    const auto maxVertexAttribs = getOpenglLimit(LimitName::MaxVertexAttribs);
    if (va.index >= maxVertexAttribs)
    {
        const auto errorMessage = std::format("Index must be less than {}.", maxVertexAttribs);
        throw std::out_of_range{errorMessage};
    }
    if (va.count < 1 || va.count > 4)
    {
        throw std::out_of_range{"Count must be greater than 0 and less than 5."};
    }

    m_impl->vertexAttributes.push_back(va);
    m_impl->stride += getByteSizeOfType(va.type) * va.count;
}

const std::vector<VertexAttribute>& VertexBufferLayout::getAttributes() const noexcept
{
    return m_impl->vertexAttributes;
}

GLsizei VertexBufferLayout::getStride() const noexcept
{
    return m_impl->stride;
}

unsigned int getByteSizeOfType(VertexAttrType type) noexcept
{
    switch (type)
    {
        case VertexAttrType::Byte:
            return 1;
        case VertexAttrType::Double:
            return 8;
        case VertexAttrType::Fixed:
            return 4;
        case VertexAttrType::Float:
            return 4;
        case VertexAttrType::HalfFloat:
            return 2;
        case VertexAttrType::Int:
            return 4;
        case VertexAttrType::Int2101010Rev:
            return 4;
        case VertexAttrType::Short:
            return 2;
        case VertexAttrType::UnsignedByte:
            return 1;
        case VertexAttrType::UnsignedInt:
            return 4;
        case VertexAttrType::UnsignedInt10f11f11fRev:
            return 4;
        case VertexAttrType::UnsignedInt2101010Rev:
            return 4;
        case VertexAttrType::UnsignedShort:
            return 2;
        default:
        {
            OGLS_ASSERT(false);
            return 1;  // like as VertexAttrType::Byte
        }
    }
}

}  // namespace ogls::oglCore::vertex
