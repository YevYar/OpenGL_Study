#ifndef OGLS_OGLCORE_VERTEX_BUFFER_H
#define OGLS_OGLCORE_VERTEX_BUFFER_H

#include <memory>
#include <optional>

#include "generalTypes.h"
#include "vertexBufferLayout.h"
#include "vertexTypes.h"

namespace ogls::oglCore::vertex
{
class VertexArray;

/**
 * \brief Buffer is a wrapper over OpenGL buffer object.
 */
class Buffer : public ICloneable
{
    private:
        /**
         * \brief Impl contains private data and methods of Buffer.
         */
        struct Impl;

    public:
        /**
         * \brief Unbinds current buffer from the target.
         * 
         * Wraps [glBindBuffer()](https://docs.gl/gl4/glBindBuffer).
         *
         * \param target - target buffer to be unbound from.
         */
        static void unbindTarget(BufferTarget target);

        /**
         * \brief Constructs new Buffer object with passed arguments, generates new 1 buffer in OpenGL state machine
         * and loads the data in referenced OpenGL buffer object.
         * 
         * Wraps [glCreateBuffers()](https://docs.gl/gl4/glCreateBuffers).
         *
         * \param target       - target to bind buffer to.
         * \param data         - data, which must be set in OpenGL buffer.
         * \param usage        - usage of data.
         * \param bufferLayout - VertexBufferLayout.
         * Use only for buffer, which is intended to be vertex buffer object. Otherwise must be std::nullopt.
         * \throw ogls::exceptions::GLRecAcquisitionException().
         */
        Buffer(BufferTarget target, ArrayData data, BufferDataUsage usage,
               std::optional<VertexBufferLayout> bufferLayout = std::nullopt);
        /**
         * \brief Constructs new Buffer as move-copy of other Buffer.
         *
         * New 1 buffer in OpenGL state machine is not generated.
         */
        Buffer(Buffer&& obj) noexcept;
        /**
         * \brief Deletes the buffer object in OpenGL state machine.
         *
         * Wraps [glDeleteBuffers()](https://docs.gl/gl4/glDeleteBuffers).
         */
        ~Buffer() noexcept;

        /**
         * \brief Move-copies the state of other Buffer.
         *
         * NOTE: Reference on pointed OpenGL buffer object of assignment destination Buffer (this object) is changed.
         * That is why VertexArray object, to which this Buffer was added, can be affected,
         * because OpenGL vertex array object represented by VertexArray object references on the previous ID
         * of this Buffer object, which was replaced by ID of Buffer obj.
         */
        Buffer& operator=(Buffer&& obj) noexcept;
        Buffer& operator=(const Buffer& obj) = delete;

        /**
         * \brief Wraps [glBindBuffer()](https://docs.gl/gl4/glBindBuffer).
         */
        void                              bind() const;
        /**
         * \brief Returns data of the Buffer.
         *          
         * \return data of the Buffer.
         */
        const ArrayData&                  getData() const noexcept;
        /**
         * \brief Returns layout of the buffer.
         * 
         * \return layout of the Buffer.
         */
        std::optional<VertexBufferLayout> getLayout() const noexcept;
        /**
         * \brief Sets new Buffer data and loads it in OpenGL buffer.
         *
         * Wraps [glNamedBufferData()](https://docs.gl/gl4/glBufferData)
         * and [glNamedBufferSubData()](https://docs.gl/gl4/glBufferSubData).
         *
         * \param data - data, which must be set in OpenGL buffer.
         */
        void                              setData(ArrayData data);
        /**
         * \brief Calls unbindTarget() with the target of the buffer.
         */
        void                              unbind() const;

        Buffer* clone() const override;

    private:
        /**
         * \brief Constructs new Buffer as copy of other Buffer.
         *
         * However new 1 buffer in OpenGL state machine is generated.
         * Deep copy of data of Buffer obj is not made (see copy constructor of ArrayData).
         * 
         * Wraps [glCreateBuffers()](https://docs.gl/gl4/glCreateBuffers).
         * 
         * \throw ogls::exceptions::GLRecAcquisitionException().
         */
        Buffer(const Buffer& obj);

    private:
        /**
         * \brief Pointer to implementation.
         */
        std::unique_ptr<Impl> m_impl;


        friend class VertexArray;

};  // class Buffer

}  // namespace ogls::oglCore::vertex

#endif
