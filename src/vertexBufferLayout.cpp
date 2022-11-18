 #include "vertexBufferLayout.h"

#include <stdexcept>

#include "vertexHelpers.h"

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
