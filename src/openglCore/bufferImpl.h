#ifndef BUFFER_IMPL_H
#define BUFFER_IMPL_H

#include "buffer.h"
#include "openglHelpers.h"

namespace vertex
{
    struct Buffer::Impl
    {
        public:
            Impl(BufferTarget target, ArrayData data, BufferDataUsage usage,
                std::optional<VertexBufferLayout> bufferLayout);
            Impl(const Impl& obj);
            Impl(Impl&& obj) noexcept;

            Impl& operator=(const Impl&) = delete;
            Impl& operator=(Impl&&) noexcept = delete;

            /**
             * \brief Deletes the object and the buffer object in OpenGL state machine.
             *
             * Wraps [glDeleteBuffers()](https://docs.gl/gl4/glDeleteBuffers).
             */
            ~Impl();

            static void bindToTarget(BufferTarget target, GLuint bufferId) noexcept;

            static BufferBindingTarget getTargetAssociatedGetParameter(BufferTarget target) noexcept;

            /**
             * \brief Generates OpenGL buffer object.
             *
             * Wraps [glGenBuffers()](https://docs.gl/gl4/glGenBuffers).
             */
            void genBuffer();

            void bind() const noexcept;

            bool checkAndGenerateNewStorage(const ArrayData& data) noexcept;

            void deleteBuffer() noexcept;

        public:
            /**
             * \brief Id of referenced OpenGL buffer.
             */
            GLuint m_rendererId = 0;
            BufferTarget m_target = BufferTarget::ARRAY_BUFFER;
            ArrayData m_data;
            BufferDataUsage m_usage = BufferDataUsage::STATIC_DRAW;
            std::optional<VertexBufferLayout> m_layout = std::nullopt;

        template<openglCore::OpenGLBindableObject Type>
        friend void openglCore::bindForAMomentAndExecute(const Type&, const std::function<void()>&);

    };
}

#endif
