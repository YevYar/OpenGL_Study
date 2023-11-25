#ifndef OGLS_OGLCORE_VERTEX_VERTEX_BUFFER_LAYOUT_IMPL_H
#define OGLS_OGLCORE_VERTEX_VERTEX_BUFFER_LAYOUT_IMPL_H

#include "vertexBufferLayout.h"

namespace ogls::oglCore::vertex
{
/**
 * \brief Impl contains private data and methods of VertexBufferLayout.
 */
class VertexBufferLayout::Impl final
{
    public:
        Impl()                = default;
        Impl(const Impl& obj) = default;
        OGLS_DEFAULT_MOVABLE(Impl)

        Impl& operator=(const Impl&) = delete;

    public:
        /**
         * \brief Automatically calculated value, when new VertexAttribute is added via
         * addVertexAttribute().
         *
         * \see VertexBufferLayout::getStride().
         */
        GLsizei                      stride = {0};
        /**
         * \brief The vector of added to layout vertex attributes.
         */
        std::vector<VertexAttribute> vertexAttributes;

};  // class VertexBufferLayout::Impl

}  // namespace ogls::oglCore::vertex

#endif
