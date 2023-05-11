#ifndef OGLS_OGLCORE_VERTEX_VERTEX_ARRAY_IMPL_H
#define OGLS_OGLCORE_VERTEX_VERTEX_ARRAY_IMPL_H

#include "vertexArray.h"

#include "helpers/macros.h"

namespace ogls::oglCore::vertex
{
/**
 * \brief Impl contains private data and methods of VertexArray.
 */
struct VertexArray::Impl
{
    public:
        /**
         * \brief Wraps [glBindVertexArray()](https://docs.gl/gl4/glBindVertexArray).
         * 
         * \param vaoId - rendererId.
         * \see VertexArray::Impl::rendererId.
         */
        static void bindSpecificVao(GLuint vaoId);

        /**
         * \see genVertexArray().
         */
        Impl();
        OGLS_NOT_COPYABLE_MOVABLE(Impl)
        /**
         * \see deleteVertexArray().
         */
        ~Impl() noexcept;

        /**
         * \brief Deletes vertex array object in OpenGL state machine.
         *
         * Wraps [glDeleteVertexArrays()](https://docs.gl/gl4/glDeleteVertexArrays).
         */
        void deleteVertexArray();
        /**
         * \brief Generates new 1 vertex array object in OpenGL state machine.
         *
         * Wraps [glCreateVertexArrays()](https://docs.gl/gl4/glCreateVertexArrays).
         * 
         * \throw ogls::exceptions::GLRecAcquisitionException().
         */
        void genVertexArray();

    public:
        /**
         * \brief ID of referenced OpenGL vertex array object.
         */
        GLuint                               rendererId = {0};
        /**
         * \brief Added to vertex array object buffers.
         */
        std::vector<std::shared_ptr<Buffer>> buffers;

};  // struct VertexArray::Impl

}  // namespace ogls::oglCore::vertex

#endif
