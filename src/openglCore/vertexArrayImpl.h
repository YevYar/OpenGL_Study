#ifndef OGLS_OGLCORE_VERTEX_VERTEX_ARRAY_IMPL_H
#define OGLS_OGLCORE_VERTEX_VERTEX_ARRAY_IMPL_H

#include "vertexArray.h"

#include "helpers/macros.h"

namespace ogls::oglCore::vertex
{
struct VertexArray::Impl
{
    public:
        static void bindSpecificVao(GLuint vaoId) noexcept;

        Impl();
        OGLS_NOT_COPYABLE_MOVABLE(Impl)
        /**
         * \brief Deletes vertex array object in OpenGL state machine.
         *
         * Wraps [glDeleteVertexArrays()](https://docs.gl/gl4/glDeleteVertexArrays).
         */
        ~Impl();

        void deleteVertexArray();
        void genVertexArray();

    public:
        /**
         * \brief Id of referenced OpenGL vertex array object.
         */
        GLuint                               rendererId = {0};
        std::vector<std::shared_ptr<Buffer>> buffers;

};  // struct VertexArray::Impl

}  // namespace ogls::oglCore::vertex

#endif
