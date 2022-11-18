#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include <vector>

namespace vertex
{
	enum class VertexAttrType
	{
		BYTE = 0x1400, UNSIGNED_BYTE = 0x1401, SHORT = 0x1402, UNSIGNED_SHORT = 0x1403,
		INT = 0x1404, UNSIGNED_INT = 0x1405, HALF_FLOAT = 0x140B, FLOAT = 0x1406,
		DOUBLE = 0x140A, INT_2_10_10_10_REV = 0x8D9F, UNSIGNED_INT_2_10_10_10_REV = 0x8368,
	};

	struct VertexAttribute
	{
		unsigned int index = 0;
		int count = 1;
		VertexAttrType type = VertexAttrType::FLOAT;
		bool normalized = false;
		int byteOffset;
	};

	class VertexBufferLayout
	{
		public:
			VertexBufferLayout() = default;
			~VertexBufferLayout() = default;

			void addVertexAttribute(const VertexAttribute& vertexAttribute);

			const std::vector<VertexAttribute>& getAttributes() const noexcept;
			unsigned int getStride() const noexcept;

		private:
			std::vector<VertexAttribute> m_vertexAttributes;
			unsigned int m_stride = 0;

	};

	unsigned int getByteSizeOfType(VertexAttrType type) noexcept;

}

#endif
