#ifndef OGLS_OGLCORE_VERTEX_BUFFER_IMPL_H
#define OGLS_OGLCORE_VERTEX_BUFFER_IMPL_H

#include "buffer.h"

#include "openglHelpersImpl.h"

namespace ogls::oglCore::vertex
{
/**
 * \brief Impl contains private data and methods of Buffer.
 */
struct Buffer::Impl
{
    public:
        /**
         * \brief Binds a buffer to a target.
         *
         * Wraps [glBindBuffer()](https://docs.gl/gl4/glBindBuffer).
         * It is necessary for ogls::oglCore::bindForAMomentAndExecute().
         * 
         * \param target   - the target to which the buffer object is bound.
         * \param bufferId - rendererId of the buffer, which must be bound.
         */
        static void                bindToTarget(BufferTarget target, GLuint bufferId);
        /**
         * \brief Returns the corresponding binding target for passed buffer target.
         * 
         * It is necessary for ogls::oglCore::bindForAMomentAndExecute().
         * 
         * \param target - the target, binding target of which is needed.
         */
        static BufferBindingTarget getTargetAssociatedGetParameter(BufferTarget target) noexcept;

        /**
         * \see genBuffer().
         */
        Impl(BufferTarget target, ArrayData data, BufferDataUsage usage,
             std::optional<VertexBufferLayout> bufferLayout);
        /**
         * \see genBuffer().
         */
        Impl(const Impl& obj);
        Impl(Impl&& obj) noexcept;
        /**
         * \see deleteBuffer().
         */
        ~Impl() noexcept;

        Impl& operator=(const Impl&) = delete;
        Impl& operator=(Impl&&)      = delete;

        /**
         * \brief Binds a buffer to a target.
         * 
         * \see bindToTarget().
         */
        void bind() const;
        /**
         * \brief Creates and initializes a buffer object's data store in OpenGL state machine.
         *
         * If [size](\ref ogls::ArrayData::size) of passed data not equal to [size](\ref ogls::ArrayData::size)
         * of current data, calls [glNamedBufferData()](https://docs.gl/gl4/glBufferData) and sets Buffer::Impl::data.
         * 
         * \return true if new storage was generated, false otherwise.
         */
        bool checkAndGenerateNewStorage(const ArrayData& data);
        /**
         * \brief Deletes the buffer object in OpenGL state machine.
         *
         * Wraps [glDeleteBuffers()](https://docs.gl/gl4/glDeleteBuffers).
         */
        void deleteBuffer();
        /**
         * \brief Generates OpenGL buffer object.
         *
         * Wraps [glCreateBuffers()](https://docs.gl/gl4/glCreateBuffers).
         * 
         * \throw ogls::exceptions::GLRecAcquisitionException().
         */
        void genBuffer();

    public:
        /**
         * \brief The data of the Buffer.
         */
        ArrayData                         data;
        /**
         * \brief The layout of the data in which it is stored in OpenGL state machine.
         */
        std::optional<VertexBufferLayout> layout     = std::nullopt;
        /**
         * \brief ID of referenced OpenGL buffer.
         */
        GLuint                            rendererId = {0};
        /**
         * \brief The buffer target, to which the buffer object is bound.
         */
        BufferTarget                      target     = BufferTarget::ArrayBuffer;
        /**
         * \brief Usage of data stored in buffer object in OpengGL state machine.
         */
        BufferDataUsage                   usage      = BufferDataUsage::StaticDraw;


        template<ogls::oglCore::OpenGLBindableObject Type>
        friend void ogls::oglCore::bindForAMomentAndExecute(const Type&, const std::function<void()>&);

};  // struct Buffer::Impl

}  // namespace ogls::oglCore::vertex

#endif
