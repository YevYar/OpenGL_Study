#ifndef VERTEX_ARRAY_IMPL_H
#define VERTEX_ARRAY_IMPL_H

#include "vertexArray.h"

namespace openglCore::vertex
{
	struct VertexArray::Impl
	{
		public:
			Impl();

			NOT_COPYABLE_MOVABLE(Impl)
		
			/**
			 * \brief Deletes vertex array object in OpenGL state machine.
			 *
			 * Wraps [glDeleteVertexArrays()](https://docs.gl/gl4/glDeleteVertexArrays).
			 */
			~Impl();

			static void bindSpecificVao(GLuint vaoId) noexcept;

			void genVertexArray();
			void deleteVertexArray();

		public:
			/**
			 * \brief Id of referenced OpenGL vertex array object.
			 */
			GLuint m_rendererId = 0;
			std::vector<std::shared_ptr<Buffer>> m_buffers;

	};
}

#endif
