#ifndef OGLS_OGLCORE_VERTEX_BUFFER_IMPL_H
#define OGLS_OGLCORE_VERTEX_BUFFER_IMPL_H

#include "buffer.h"

#include "openglHelpersImpl.h"

namespace ogls::oglCore::vertex
{
struct Buffer::Impl
{
    public:
        static void                bindToTarget(BufferTarget target, GLuint bufferId) noexcept;
        static BufferBindingTarget getTargetAssociatedGetParameter(BufferTarget target) noexcept;

        Impl(BufferTarget target, ArrayData data, BufferDataUsage usage,
             std::optional<VertexBufferLayout> bufferLayout);
        Impl(const Impl& obj);
        Impl(Impl&& obj) noexcept;
        /**
         * \brief Deletes the object and the buffer object in OpenGL state machine.
         *
         * Wraps [glDeleteBuffers()](https://docs.gl/gl4/glDeleteBuffers).
         */
        ~Impl();

        Impl& operator=(const Impl&)     = delete;
        Impl& operator=(Impl&&) noexcept = delete;

        void bind() const noexcept;
        bool checkAndGenerateNewStorage(const ArrayData& data) noexcept;
        void deleteBuffer() noexcept;
        /**
         * \brief Generates OpenGL buffer object.
         *
         * Wraps [glGenBuffers()](https://docs.gl/gl4/glGenBuffers).
         */
        void genBuffer();

    public:
        ArrayData                         data;
        std::optional<VertexBufferLayout> layout     = std::nullopt;
        /**
         * \brief Id of referenced OpenGL buffer.
         */
        GLuint                            rendererId = {0};
        BufferTarget                      target     = BufferTarget::ArrayBuffer;
        BufferDataUsage                   usage      = BufferDataUsage::StaticDraw;


        template<ogls::oglCore::OpenGLBindableObject Type>
        friend void ogls::oglCore::bindForAMomentAndExecute(const Type&, const std::function<void()>&);

};  // struct Buffer::Impl

}  // namespace ogls::oglCore::vertex

#endif
