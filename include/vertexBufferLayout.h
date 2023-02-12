#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include <vector>

#include "helpers/macros.h"

namespace vertex
{
	/**
	 * VertexAttrType represents 'type' parameter of 
	 * [glVertexAttribPointer()](https://docs.gl/gl3/glVertexAttribPointer).
	 */
	enum class VertexAttrType : unsigned int
	{
		BYTE = 0x1400, UNSIGNED_BYTE = 0x1401, SHORT = 0x1402, UNSIGNED_SHORT = 0x1403,
		INT = 0x1404, UNSIGNED_INT = 0x1405, HALF_FLOAT = 0x140B, FLOAT = 0x1406,
		DOUBLE = 0x140A, INT_2_10_10_10_REV = 0x8D9F, UNSIGNED_INT_2_10_10_10_REV = 0x8368,
	};

	/**
	 * \brief VertexAttribute represents one vertex attribute and contains all arguments,
	 * which are needed to call function [glVertexAttribPointer()](https://docs.gl/gl3/glVertexAttribPointer).
	 */
	struct VertexAttribute
	{
		/**
		 * \brief The index of the generic vertex attribute to be modified.
		 */
		unsigned int index = 0;
		/**
		 * \brief The number of components per generic vertex attribute. Must be 1, 2, 3, 4.
		 */
		int count = 1;
		/**
		 * \brief The data type of each component in the array.
		 */
		VertexAttrType type = VertexAttrType::FLOAT;
		/**
		 * \brief Specification whether fixed-point data values should be normalized or converted directly 
		 * as fixed-point values when they are accessed.
		 */
		bool normalized = false;
		/**
		 * \brief The 'pointer' parameter of the function. Specifies an offset of the first component 
		 * of the first generic vertex attribute in the array in the data store of the buffer currently bound 
		 * to the GL_ARRAY_BUFFER target.
		 */
		int byteOffset = 0;
	};

	/**
	 * \brief VertexBufferLayout represents the format of the data, which is stored in a Buffer.
	 */
	class VertexBufferLayout
	{
		public:
			VertexBufferLayout() = default;
			~VertexBufferLayout() = default;

			DEFAULT_COPYABLE_MOVABLE(VertexBufferLayout)

			/**
			 * \brief Adds new VertexAttribute in layout.
			 * 
			 * \param vertexAttribute - vertex attribute, which must be added to the vertex buffer layout.
			 * \throw std::invalid_argument
			 */
			void addVertexAttribute(const VertexAttribute& vertexAttribute);

			/**
			 * \brief Returns all VertexAttribute of layout.
			 */
			const std::vector<VertexAttribute>& getAttributes() const noexcept;

			/**
			 * \brief Returns automatically calculated 'stride' parameter, which is needed by 
			 * [glVertexAttribPointer()](https://docs.gl/gl3/glVertexAttribPointer).
			 */
			unsigned int getStride() const noexcept;

		private:
			std::vector<VertexAttribute> m_vertexAttributes;

			/**
			 * \brief See getStride(). It is automatically calculated value, when new VertexAttribute is added via addVertexAttribute().
			 */
			unsigned int m_stride = 0;

	};

    /**
     * \brief Returns the size in bytes of the type accordingly to 
     * [table of OpenGL types](https://www.khronos.org/opengl/wiki/OpenGL_Type).
     * 
     * \param type - type to get the size of.
     * \return size in bytes of the type.
     */
	unsigned int getByteSizeOfType(VertexAttrType type) noexcept;

}

#endif
