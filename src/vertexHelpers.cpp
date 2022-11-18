#include "vertexHelpers.h"

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
