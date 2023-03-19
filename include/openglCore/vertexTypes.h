#ifndef VERTEX_TYPES_H
#define VERTEX_TYPES_H

#include <glad/glad.h>

namespace openglCore::vertex
{
    /**
     * \brief BufferTarget represents 'target' parameter of [glBindBuffer()](https://docs.gl/gl4/glBindBuffer).
     */
    enum class BufferTarget : GLenum
    {
        ARRAY_BUFFER = 0x8892, ATOMIC_COUNTER_BUFFER = 0x92C0, COPY_READ_BUFFER = 0x8F36, COPY_WRITE_BUFFER = 0x8F37,
        DISPATCH_INDIRECT_BUFFER = 0x90EE, DRAW_INDIRECT_BUFFER = 0x8F3F, ELEMENT_ARRAY_BUFFER = 0x8893,
        PIXEL_PACK_BUFFER = 0x88EB, PIXEL_UNPACK_BUFFER = 0x88EC, QUERY_BUFFER = 0x9192, SHADER_STORAGE_BUFFER = 0x90D2,
        TEXTURE_BUFFER = 0x8C2A, TRANSFORM_FEEDBACK_BUFFER = 0x8C8E, UNIFORM_BUFFER = 0x8A11
    };

    enum class BufferBindingTarget : GLenum
    {
        ARRAY_BUFFER_BINDING = 0x8894, ATOMIC_COUNTER_BUFFER_BINDING = 0x92C1, COPY_READ_BUFFER_BINDING = 0x8F36,
        COPY_WRITE_BUFFER_BINDING = 0x8F37, DRAW_INDIRECT_BUFFER_BINDING = 0x8F43,
        DISPATCH_INDIRECT_BUFFER_BINDING = 0x90EF, ELEMENT_ARRAY_BUFFER_BINDING = 0x8895,
        PIXEL_PACK_BUFFER_BINDING = 0x88ED, PIXEL_UNPACK_BUFFER_BINDING = 0x88EF,
        SHADER_STORAGE_BUFFER_BINDING = 0x90D3, QUERY_BUFFER_BINDING = 0x9193, TEXTURE_BUFFER_BINDING = 0x8C2A,
        TRANSFORM_FEEDBACK_BUFFER_BINDING = 0x8C8F, UNIFORM_BUFFER_BINDING = 0x8A28
    };

    /**
     * \brief BufferDataUsage represents 'usage' parameter of [glBufferData()](https://docs.gl/gl4/glBufferData).
     */
    enum class BufferDataUsage : GLenum
    {
        STREAM_DRAW = 0x88E0, STREAM_READ = 0x88E1, STREAM_COPY = 0x88E2, STATIC_DRAW = 0x88E4,
        STATIC_READ = 0x88E5, STATIC_COPY = 0x88E6, DYNAMIC_DRAW = 0x88E8, DYNAMIC_READ = 0x88E9,
        DYNAMIC_COPY = 0x88EA
    };

    /**
     * VertexAttrType represents 'type' parameter of
     * [glVertexAttribPointer()](https://docs.gl/gl4/glVertexAttribPointer).
     */
    enum class VertexAttrType : GLenum
    {
        BYTE = 0x1400, UNSIGNED_BYTE = 0x1401, SHORT = 0x1402, UNSIGNED_SHORT = 0x1403,
        INT = 0x1404, UNSIGNED_INT = 0x1405, HALF_FLOAT = 0x140B, FLOAT = 0x1406,
        DOUBLE = 0x140A, FIXED = 0x140C, INT_2_10_10_10_REV = 0x8D9F, UNSIGNED_INT_2_10_10_10_REV = 0x8368,
        UNSIGNED_INT_10F_11F_11F_REV = 0x8C3B
    };
}

#endif
