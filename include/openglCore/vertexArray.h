#ifndef OGLS_OGLCORE_VERTEX_VERTEX_ARRAY_H
#define OGLS_OGLCORE_VERTEX_VERTEX_ARRAY_H

#include <memory>
#include <vector>

#include <glad/glad.h>

#include "buffer.h"
#include "generalTypes.h"

/**
 * \namespace oglCore
 * \brief oglCore namespace contains types and functions, which represent OO-wrapper over OpenGL types and functions.
 */
/**
 * \namespace vertex
 * \brief vertex namespace contains types and functions, which are related to vertex array objects,
 * vertex buffer objects etc.
 */
namespace ogls::oglCore::vertex
{
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
    private:
        /**
         * \brief Impl contains private data and methods of VertexArray.
         */
        struct Impl;

    public:
        /**
         * \brief Unbinds current vertex array object.
         */
        static void unbind() noexcept;

        /**
         * \brief Constructs new VertexArray object and generates new 1 vertex array object in OpenGL state machine.
         *
         * This vertex array object is bound to become active vertex array object (see bind()).
         *
         * Wraps [glGenVertexArrays()](https://docs.gl/gl4/glGenVertexArrays).
         */
        VertexArray();
        VertexArray(VertexArray&& obj) noexcept;
        ~VertexArray();

        VertexArray& operator=(VertexArray&& obj) noexcept;
        VertexArray& operator=(const VertexArray& obj) = delete;

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
        void                                        addBuffer(std::shared_ptr<Buffer> buffer) noexcept;
        /**
         * \brief Wraps [glBindVertexArray()](https://docs.gl/gl4/glBindVertexArray).
         */
        void                                        bind() const noexcept;
        /**
         * \brief Wraps [glDisableVertexAttribArray()](https://docs.gl/gl4/glDisableVertexAttribArray).
         *
         * This vertex array object is bound to become active vertex array object (see bind()).
         *
         * \param index - the index of the generic vertex attribute to be disabled.
         */
        void                                        disableAttribute(unsigned int index) const noexcept;
        /**
         * \brief Wraps [glEnableVertexAttribArray()](https://docs.gl/gl4/glEnableVertexAttribArray).
         *
         * This vertex array object is bound to become active vertex array object (see bind()).
         *
         * \param index - the index of the generic vertex attribute to be enabled.
         */
        void                                        enableAttribute(unsigned int index) const noexcept;
        /**
         * \brief Returns all bound buffers.
         */
        const std::vector<std::shared_ptr<Buffer>>& getBuffers() const noexcept;

        VertexArray* clone() const override;

    private:
        VertexArray(const VertexArray& obj);

    private:
        /**
         * \brief Pointer to implementation.
         */
        std::unique_ptr<Impl> m_impl;

};  // class VertexArray

}  // namespace ogls::oglCore::vertex

#endif
