#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <glad/glad.h>
#include <memory>
#include <vector>

#include "generalTypes.h"
#include "helpers/macros.h"

/**
 * \brief vertex namespace contains types and functions, which are related to vertex array objects,
 * vertex buffer objects etc.
 */
namespace vertex
{
	class Buffer;
	
	/**
	 * \brief VertexArray is a wrapper over OpenGL vertex array object.
	 * 
	 * This class is not copyable or movable. This is as it is, because OpenGL vertex array object is intended to be used
	 * to save settings to render the same objects. If objects of this class would be copyable, 
	 * the first call of destructor of one of copied objects cause deletion of OpenGL vertex array object, 
	 * specified by m_rendererId, and next renders would fail. Implementing the same logic as in Buffer class, 
	 * when all constructors generate new OpenGL buffer, conflicts with the purpose of using OpenGL vertex array object.
	 * 
	 * So, VertexArray should be stored as std::shared_ptr.
	 */
	class VertexArray : public ICloneable
	{
		public:
			/**
			 * \brief Constructs new VertexArray object and generates new 1 vertex array object in OpenGL state machine.
			 * 
			 * This vertex array object is bound to become active vertex array object (see bind()).
			 * 
			 * Wraps [glGenVertexArrays()](https://docs.gl/gl4/glGenVertexArrays).
			 */
			VertexArray();

            VertexArray(VertexArray&& obj) noexcept;

			/**
			 * \brief Deletes vertex array object in OpenGL state machine.
			 * 
			 * Wraps [glDeleteVertexArrays()](https://docs.gl/gl4/glDeleteVertexArrays).
			 */
			~VertexArray();	

			VertexArray& operator=(const VertexArray& obj) = delete;
            VertexArray& operator=(VertexArray&& obj) noexcept;

            /**
             * \brief Unbinds current vertex array object.
             */
            static void unbind() noexcept;

			VertexArray* clone() const override;

			/**
			 * \brief Wraps [glBindVertexArray()](https://docs.gl/gl4/glBindVertexArray).
			 */
			void bind() const noexcept;

			/**
			 * \brief Binds the buffer to OpenGL vertex array object.
			 * 
			 * First of all this vertex array object is bound to become active vertex array object (see bind()).
			 * After that the buffer is bound to this vertex array object (see Buffer::bind()).
			 * If buffer has layout, [glVertexAttribPointer()](https://docs.gl/gl4/glVertexAttribPointer) is called.
			 * Every attribute is automatically enabled (see enableAttribute()).
			 * 
			 * OpenGL buffer, which is wrapped in Buffer class, can be bound to currently bound vertex array object
			 * by calling Buffer::bind(). However, it is not recommended approach, addBuffer() is preferred.
			 * 
			 * \param buffer - buffer to be bound to this VertexArray.
			 */
			void addBuffer(std::shared_ptr<Buffer> buffer) noexcept;

			/**
			 * \brief Returns all bound buffers.
			 */
			const std::vector<std::shared_ptr<Buffer>>& getBuffers() const noexcept;

			/**
			 * \brief Wraps [glEnableVertexAttribArray()](https://docs.gl/gl4/glEnableVertexAttribArray).
			 * 
			 * This vertex array object is bound to become active vertex array object (see bind()).
			 * 
			 * \param index - the index of the generic vertex attribute to be enabled.
			 */
			void enableAttribute(unsigned int index) const noexcept;

			/**
			 * \brief Wraps [glDisableVertexAttribArray()](https://docs.gl/gl4/glDisableVertexAttribArray).
			 *
			 * This vertex array object is bound to become active vertex array object (see bind()).
			 * 
			 * \param index - the index of the generic vertex attribute to be disabled.
			 */
			void disableAttribute(unsigned int index) const noexcept;

        private:
			VertexArray(const VertexArray& obj);

            static void bindSpecificVao(GLuint vaoId) noexcept;

			void genVertexArray();
            void deleteVertexArray();

		private:
			/**
			 * \brief Id of referenced OpenGL vertex array object.
			 */
            GLuint m_rendererId = 0;
			std::vector<std::shared_ptr<Buffer>> m_buffers;
			
	};
}

#endif
