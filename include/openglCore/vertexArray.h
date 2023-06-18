#ifndef OGLS_OGLCORE_VERTEX_VERTEX_ARRAY_H
#define OGLS_OGLCORE_VERTEX_VERTEX_ARRAY_H

#include <memory>
#include <vector>

#include <glad/glad.h>

#include "buffer.h"
#include "generalTypes.h"

/**
 * \namespace ogls::oglCore
 * \brief oglCore namespace contains types and functions, which represent OO-wrapper over OpenGL types and functions.
 */
/**
 * \namespace ogls::oglCore::vertex
 * \brief vertex namespace contains types and functions, which are related to vertex array objects,
 * vertex buffer objects etc.
 */
namespace ogls::oglCore::vertex
{
/**
 * \brief VertexArray is a wrapper over OpenGL vertex array object.
 */
class VertexArray : public ICloneable
{
    private:
        /**
         * \brief Impl contains private data and methods of VertexArray.
         */
        class Impl;

    public:
        /**
         * \brief Unbinds current vertex array object.
         *
         * Wraps [glBindVertexArray()](https://docs.gl/gl4/glBindVertexArray).
         */
        static void unbind();

        /**
         * \brief Constructs new VertexArray object and generates new 1 vertex array object in OpenGL state machine.
         *
         * This vertex array object is bound to become active vertex array object (see bind()).
         *
         * Wraps [glCreateVertexArrays()](https://docs.gl/gl4/glCreateVertexArrays).
         *
         * \see bind().
         * \throw ogls::exceptions::GLRecAcquisitionException().
         */
        VertexArray();
        /**
         * \brief Constructs new VertexArray as move-copy of other VertexArray.
         *
         * New 1 vertex array object in OpenGL state machine is not generated.
         */
        VertexArray(VertexArray&& obj) noexcept;
        /**
         * \brief Deletes the vertex array object in OpenGL state machine.
         *
         * Wraps [glDeleteVertexArrays()](https://docs.gl/gl4/glDeleteVertexArrays).
         */
        ~VertexArray() noexcept;

        /**
         * \brief Move-copies the state of other VertexArray.
         */
        VertexArray& operator=(VertexArray&& obj) noexcept;
        VertexArray& operator=(const VertexArray& obj) = delete;

        /**
         * \brief Binds the buffer to OpenGL vertex array object.
         *
         * If buffer has no layout, this vertex array object is bound (if needed) to become active vertex array object
         * (see bind()) and buffer.[bind()](\ref Buffer::bind()) is called. After that previous bound VAO is bound back.
         *
         * If buffer has layout, buffer data is loaded using
         * [glVertexArrayVertexBuffer()](https://docs.gl/gl4/glBindVertexBuffer),
         * [glVertexArrayAttribBinding()](https://docs.gl/gl4/glVertexAttribBinding),
         * and [glVertexArrayAttribFormat()](https://docs.gl/gl4/glVertexAttribFormat)
         * in the format (VNCVNCVNCVNC)
         * (see [Formatting VBO
         * Data](https://www.khronos.org/opengl/wiki/Vertex_Specification_Best_Practices#Formatting_VBO_Data)).
         *
         * Every attribute is automatically enabled (see enableAttribute()).
         *
         * OpenGL buffer, which is wrapped in Buffer class, can be bound to currently bound vertex array object
         * by calling Buffer::bind(). However, it is not recommended approach, addBuffer() is preferred.
         *
         * \see bind(), enableAttribute().
         * \param buffer - buffer to be bound to this VertexArray.
         */
        void                                        addBuffer(std::shared_ptr<Buffer> buffer);
        /**
         * \brief Wraps [glBindVertexArray()](https://docs.gl/gl4/glBindVertexArray).
         */
        void                                        bind() const;
        /**
         * \brief Wraps [glDisableVertexAttribArray()](https://docs.gl/gl4/glDisableVertexAttribArray).
         *
         * \param index - the index of the generic vertex attribute to be disabled.
         */
        void                                        disableAttribute(int index);
        /**
         * \brief Wraps [glEnableVertexAttribArray()](https://docs.gl/gl4/glEnableVertexAttribArray).
         *
         * \param index - the index of the generic vertex attribute to be enabled.
         */
        void                                        enableAttribute(int index);
        /**
         * \brief Returns all bound buffers.
         */
        const std::vector<std::shared_ptr<Buffer>>& getBuffers() const noexcept;

        VertexArray* clone() const override;

    private:
        /**
         * \brief Constructs new VertexArray as copy of other VertexArray.
         *
         * However new 1 vertex array object is generated in OpenGL state machine. All buffers of obj are
         * added to new VertexArray object (see addBuffer()).
         *
         * Wraps [glCreateVertexArrays()](https://docs.gl/gl4/glCreateVertexArrays).
         *
         * \see addBuffer().
         * \throw ogls::exceptions::GLRecAcquisitionException().
         */
        VertexArray(const VertexArray& obj);

    private:
        /**
         * \brief Pointer to implementation.
         */
        std::unique_ptr<Impl> m_impl;

};  // class VertexArray

}  // namespace ogls::oglCore::vertex

#endif
