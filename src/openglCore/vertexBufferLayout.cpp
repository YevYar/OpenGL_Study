#include "vertexBufferLayout.h"
#include "vertexBufferLayoutImpl.h"

#include <format>
#include <glad/glad.h>
#include <stdexcept>

#include "openglLimits.h"

using namespace vertex;

VertexBufferLayout::VertexBufferLayout() : m_impl{ std::make_unique<Impl>() }
{
}

VertexBufferLayout::VertexBufferLayout(const VertexBufferLayout& obj) : m_impl{ std::make_unique<Impl>(*obj.m_impl) }
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
    const GLint maxVertexAttribs = getOpenglLimit(LimitName::MAX_VERTEX_ATTRIBS);
    if (va.index >= maxVertexAttribs)
    {
        const auto errorMessage = std::format("Index must be less than {}.", maxVertexAttribs);
        throw std::out_of_range(errorMessage);
    }
	if (va.count < 1 || va.count > 4)
	{
		throw std::out_of_range("Count must be greater than 0 and less than 5.");
	}

	m_impl->m_vertexAttributes.push_back(va);
	m_impl->m_stride += getByteSizeOfType(va.type) * va.count;
}

const std::vector<VertexAttribute>& VertexBufferLayout::getAttributes() const noexcept
{
	return m_impl->m_vertexAttributes;
}

GLsizei VertexBufferLayout::getStride() const noexcept
{
	return m_impl->m_stride;
}

unsigned int vertex::getByteSizeOfType(VertexAttrType type) noexcept
{
	switch (type)
	{
		case VertexAttrType::BYTE:
			return 1;
		case VertexAttrType::UNSIGNED_BYTE:
			return 1;
		case VertexAttrType::SHORT:
			return 2;
		case VertexAttrType::UNSIGNED_SHORT:
			return 2;
		case VertexAttrType::INT:
			return 4;
		case VertexAttrType::UNSIGNED_INT:
			return 4;
		case VertexAttrType::HALF_FLOAT:
			return 2;
		case VertexAttrType::FLOAT:
			return 4;
		case VertexAttrType::DOUBLE:
			return 8;
		case VertexAttrType::INT_2_10_10_10_REV:
			return 4;
		case VertexAttrType::UNSIGNED_INT_2_10_10_10_REV:
			return 4;
	}
}


// IMPLEMENTATION

VertexBufferLayout::Impl::Impl(const Impl& obj) : m_stride{ obj.m_stride },
	m_vertexAttributes{ obj.m_vertexAttributes }
{
}
