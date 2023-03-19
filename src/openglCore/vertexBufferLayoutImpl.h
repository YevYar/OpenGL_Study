#ifndef VERTEX_BUFFER_LAYOUT_IMPL_H
#define VERTEX_BUFFER_LAYOUT_IMPL_H

#include "vertexBufferLayout.h"

namespace openglCore::vertex
{
	struct VertexBufferLayout::Impl
	{
		public:
			Impl() = default;
			Impl(const Impl& obj);

			DEFAULT_MOVABLE(Impl)

			Impl& operator=(const Impl&) = delete;
		
		public:
			std::vector<VertexAttribute> m_vertexAttributes;

			/**
			 * \brief See getStride(). It is automatically calculated value, when new VertexAttribute is added via addVertexAttribute().
			 */
			GLsizei m_stride = 0;

	};
}

#endif
