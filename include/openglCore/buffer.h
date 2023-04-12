#ifndef OGLS_OGLCORE_VERTEX_BUFFER_H
#define OGLS_OGLCORE_VERTEX_BUFFER_H

#include <memory>
#include <optional>

#include "generalTypes.h"
#include "vertexArray.h"
#include "vertexBufferLayout.h"
#include "vertexTypes.h"

namespace ogls::oglCore::vertex
{
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
         * \param target - target buffer to be unbound from.
         */
        static void unbindTarget(BufferTarget target) noexcept;

        /**
         * \brief Constructs new Buffer object with passed arguments and
         * generates new 1 buffer in OpenGL state machine.
         *
         * Buffer data specified by data parameter is not immediately loaded in generated OpenGL buffer.
         * Data is loaded in generated OpenGL buffer in first call of method bind().
         *
         * \param target       - target to bind buffer to.
         * \param data         - data, which must be set in OpenGL buffer.
         * \param usage        - usage of data.
         * \param bufferLayout - VertexBufferLayout.
         * Use only for buffer, which is intended to be vertex buffer object. Otherwise must be std::nullopt.
         *
         * \throw exceptions::GLRecAcquisitionException().
         */
        Buffer(BufferTarget target, ArrayData data, BufferDataUsage usage,
               std::optional<VertexBufferLayout> bufferLayout = std::nullopt);
        /**
         * \brief Constructs new Buffer as move-copy of other Buffer.
         *
         * New 1 buffer in OpenGL state machine is not generated.
         *
         * If data of moved Buffer was not loaded early,
         * data is loaded in OpenGL buffer in first call of method bind().
         */
        Buffer(Buffer&& obj) noexcept;
        ~Buffer();

        /**
         * \brief Move-copies the state of other Buffer.
         *
         * Reference on pointed OpenGL buffer of assignment destination Buffer is not changed.
         *
         * Data is loaded in OpenGL buffer of assignment destination Buffer in first call of method bind().
         */
        Buffer& operator=(Buffer&& obj) noexcept;
        // Note that it can affect VAO to which current buffer is bound, because OpenGL buffer obj. can references on old buffer
        Buffer& operator=(const Buffer& obj) = delete;

        /**
         * \brief Wraps [glBindBuffer()](https://docs.gl/gl4/glBindBuffer).
         *
         * Also sets Buffer data in OpenGL buffer if it was not set before.
         */
        void bind() const noexcept;
        /**
         * \brief Returns data of the Buffer.
         *
         * It is not guaranteed that data of the Buffer is loaded in OpenGL buffer.
         *
         * Loading of data is done by:
         * - first call of bind() after object creation or assignment,
         * - calling of setData().
         *
         * \return data of the Buffer.
         */
        const ArrayData& getData() const noexcept;
        /**
         * \brief Returns layout of the buffer.
         */
        std::optional<VertexBufferLayout> getLayout() const noexcept;
        /**
         * \brief Sets new Buffer data and loads it in OpenGL buffer.
         *
         * The buffer is immediately bound (see bind()) and data is loaded.
         *
         * \param data - data, which must be set in OpenGL buffer.
         */
        void setData(ArrayData data);
        /**
         * \brief Calls unbindTarget() with the target of the buffer.
         */
        void unbind() const noexcept;

        Buffer* clone() const override;

    private:
        /**
         * \brief Constructs new Buffer as copy of other Buffer.
         *
         * However new 1 buffer in OpenGL state machine is generated.
         * Deep copy of m_data is not made (see copy constructor of ArrayData).
         *
         * Buffer data is not immediately loaded in generated OpenGL buffer.
         * Data is loaded in generated OpenGL buffer in first call of method bind().
         *
         * \throw exceptions::GLRecAcquisitionException().
         */
        Buffer(const Buffer& obj);

    private:
        std::unique_ptr<Impl> m_impl;


        friend class VertexArray;

};  // class Buffer

}  // namespace ogls::oglCore::vertex

#endif
