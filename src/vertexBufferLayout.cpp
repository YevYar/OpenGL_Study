 #include "vertexBufferLayout.h"

#include <stdexcept>

using namespace vertex;

void VertexBufferLayout::addVertexAttribute(const VertexAttribute& va)
{
	if (va.index < 0)
	{
		throw std::invalid_argument("Index must be equal to 0 or be greater.");
	}
	if (va.count < 1 || va.count > 4)
	{
		throw std::invalid_argument("Count must be greater than 0 and less than 5.");
	}

	m_vertexAttributes.push_back(va);
	m_stride += getByteSizeOfType(va.type) * va.count;
}

const std::vector<VertexAttribute>& VertexBufferLayout::getAttributes() const noexcept
{
	return m_vertexAttributes;
}

unsigned int VertexBufferLayout::getStride() const noexcept
{
	return m_stride;
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
