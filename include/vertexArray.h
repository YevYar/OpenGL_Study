#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <memory>
#include <vector>

#include "helpers/macros.h"

namespace vertex
{
	class Buffer;
	
	/*!
	 * \brief VertexArray is a wrapper for OpenGL vertex array object.
	 */
	class VertexArray
	{
		public:
			/**
			 * \brief Constructs new VertexArray object and generates new 1 vertex array object in OpenGL state machine.
			 * 
			 * This vertex array object is bound to become active vertex array object (see bind()).
			 */
			VertexArray();

			/**
			 * \brief Destructor, which deletes vertex array object in OpenGL state machine.
			 */
			~VertexArray();

			NOT_COPYABLE_MOVABLE(VertexArray)

			/**
			 * \brief Wrapper for [glBindVertexArray](https://docs.gl/gl3/glBindVertexArray).
			 */
			void bind() const noexcept;
			void unbind() const noexcept;

			/**
			 * \brief Binds the buffer to OpenGL vertex array object.
			 * 
			 * First of all this vertex array object is bound to become active vertex array object (see bind()).
			 * After that the buffer is bound to this vertex array object (see Buffer::bind()).
			 * If buffer has layout, [glVertexAttribPointer](https://docs.gl/gl3/glVertexAttribPointer) is called.
			 * Every attribute is automatically enabled (see enableAttribute()).
			 * 
			 * OpenGL buffer, which is wrapped in Buffer class, can be bound to currently bound vertex array object
			 * by calling Buffer::bind(). However, it is not recommended approach, addBuffer() is preferred. 
			 */
			void addBuffer(std::shared_ptr<Buffer> buffer) noexcept;

			/**
			 * \brief Wrapper for [glEnableVertexAttribArray](https://docs.gl/gl3/glEnableVertexAttribArray).
			 * 
			 * This vertex array object is bound to become active vertex array object (see bind()).
			 */
			void enableAttribute(unsigned int index) const noexcept;

			/**
			 * \brief Wrapper for [glDisableVertexAttribArray](https://docs.gl/gl3/glDisableVertexAttribArray).
			 *
			 * This vertex array object is bound to become active vertex array object (see bind()).
			 */
			void disableAttribute(unsigned int index) const noexcept;

		private:
			/**
			 * \brief Id of referenced OpenGL vertex array object.
			 */
			unsigned int m_rendererId = 0;
			std::vector<std::shared_ptr<Buffer>> m_buffers;
			
	};
}

#endif
