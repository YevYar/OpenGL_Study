#ifndef OGLS_OGLCORE_VERTEX_VERTEX_BUFFER_LAYOUT_H
#define OGLS_OGLCORE_VERTEX_VERTEX_BUFFER_LAYOUT_H

#include <memory>
#include <vector>

#include "helpers/macros.h"
#include "vertexTypes.h"

namespace ogls::oglCore::vertex
{
/**
 * \brief VertexAttribute represents one vertex attribute and contains all arguments,
 * which are needed to call function [glVertexAttribPointer()](https://docs.gl/gl4/glVertexAttribPointer).
 */
struct VertexAttribute
{
        /**
         * \brief The 'pointer' parameter of the function. Specifies an offset in bytes of the first component
         * of the first generic vertex attribute in the array in the data store of the buffer currently bound
         * to the GL_ARRAY_BUFFER target.
         */
        int   byteOffset = {0};
        /**
         * \brief The number of components per generic vertex attribute. Must be 1, 2, 3, 4.
         */
        GLint          count      = {1};
        /**
         * \brief The index of the generic vertex attribute to be modified.
         */
        GLuint         index      = {0};
        /**
         * \brief Specification whether fixed-point data values should be normalized or converted directly
         * as fixed-point values when they are accessed.
         */
        GLboolean      normalized = false;
        /**
         * \brief The data type of each component in the array.
         */
        VertexAttrType type       = VertexAttrType::Float;

};  // struct VertexAttribute

/**
 * \brief VertexBufferLayout represents the format of the data, which is stored in a Buffer.
 * 
 * The format is (VNCVNCVNCVNC)
 * (see [Formatting VBO Data](https://www.khronos.org/opengl/wiki/Vertex_Specification_Best_Practices#Formatting_VBO_Data)).
 * 
 * \see ogls::oglCore::vertex::VertexArray::addBuffer().
 */
class VertexBufferLayout
{
    private:
        /**
         * \brief Impl contains private data and methods of VertexBufferLayout.
         */
        struct Impl;

    public:
        OGLS_DEFAULT_MOVABLE(VertexBufferLayout)
        /**
         * \brief Constructs new layout without any vertex attributes.
         */
        VertexBufferLayout();
        VertexBufferLayout(const VertexBufferLayout& obj);
        ~VertexBufferLayout() noexcept;

        VertexBufferLayout& operator=(const VertexBufferLayout& obj);

        /**
         * \brief Adds new VertexAttribute in layout.
         *
         * \param vertexAttribute - vertex attribute, which must be added to the vertex buffer layout.
         * \throw std::out_of_range.
         */
        void                                addVertexAttribute(const VertexAttribute& vertexAttribute);
        /**
         * \brief Returns all VertexAttribute of layout.
         */
        const std::vector<VertexAttribute>& getAttributes() const noexcept;
        /**
         * \brief Returns automatically calculated 'stride' parameter, which is needed by
         * [glVertexAttribPointer()](https://docs.gl/gl4/glVertexAttribPointer).
         */
        GLsizei                             getStride() const noexcept;

    private:
        /**
         * \brief Pointer to implementation.
         */
        std::unique_ptr<Impl> m_impl;

};  // class VertexBufferLayout

/**
 * \brief Returns the size in bytes of the type accordingly to
 * [table of OpenGL types](https://www.khronos.org/opengl/wiki/OpenGL_Type).
 *
 * \param type - type to get the size of.
 * \return size in bytes of the type.
 */
int getByteSizeOfType(VertexAttrType type) noexcept;

}  // namespace ogls::oglCore::vertex

#endif
