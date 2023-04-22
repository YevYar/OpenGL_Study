#ifndef OGLS_OGLCORE_VERTEX_VERTEX_TYPES_H
#define OGLS_OGLCORE_VERTEX_VERTEX_TYPES_H

#include <glad/glad.h>

namespace ogls::oglCore::vertex
{
/**
 * \brief BufferTarget represents 'target' parameter of [glBindBuffer()](https://docs.gl/gl4/glBindBuffer).
 */
enum class BufferTarget : GLenum
{
    ArrayBuffer             = 0x88'92,
    AtomicCounterBuffer     = 0x92'C0,
    CopyReadBuffer          = 0x8F'36,
    CopyWriteBuffer         = 0x8F'37,
    DispatchIndirectBuffer  = 0x90'EE,
    DrawIndirectBuffer      = 0x8F'3F,
    ElementArrayBuffer      = 0x88'93,
    PixelPackBuffer         = 0x88'EB,
    PixelUnpackBuffer       = 0x88'EC,
    QueryBuffer             = 0x91'92,
    ShaderStorageBuffer     = 0x90'D2,
    TextureBuffer           = 0x8C'2A,
    TransformFeedbackBuffer = 0x8C'8E,
    UniformBuffer           = 0x8A'11
};

/**
 * \brief BufferBindingTarget is a set of values of 'pname' parameter of [glGetIntegerv()](https://docs.gl/gl4/glGet)
 * to get ID of current bound object to specific BufferTarget.
 */
enum class BufferBindingTarget : GLenum
{
    ArrayBufferBinding             = 0x88'94,
    AtomicCounterBufferBinding     = 0x92'C1,
    CopyReadBufferBinding          = 0x8F'36,
    CopyWriteBufferBinding         = 0x8F'37,
    DispatchIndirectBufferBinding  = 0x90'EF,
    DrawIndirectBufferBinding      = 0x8F'43,
    ElementArrayBufferBinding      = 0x88'95,
    PixelPackBufferBinding         = 0x88'ED,
    PixelUnpackBufferBinding       = 0x88'EF,
    QueryBufferBinding             = 0x91'93,
    ShaderStorageBufferBinding     = 0x90'D3,
    TextureBufferBinding           = 0x8C'2A,
    TransformFeedbackBufferBinding = 0x8C'8F,
    UniformBufferBinding           = 0x8A'28
};

/**
 * \brief BufferDataUsage represents 'usage' parameter of [glBufferData()](https://docs.gl/gl4/glBufferData).
 */
enum class BufferDataUsage : GLenum
{
    DynamicCopy = 0x88'EA,
    DynamicDraw = 0x88'E8,
    DynamicRead = 0x88'E9,
    StaticCopy  = 0x88'E6,
    StaticDraw  = 0x88'E4,
    StaticRead  = 0x88'E5,
    StreamCopy  = 0x88'E2,
    StreamDraw  = 0x88'E0,
    StreamRead  = 0x88'E1
};

/**
 * VertexAttrType represents 'type' parameter of
 * [glVertexAttribPointer()](https://docs.gl/gl4/glVertexAttribPointer).
 */
enum class VertexAttrType : GLenum
{
    Byte                    = 0x14'00,
    Double                  = 0x14'0A,
    Fixed                   = 0x14'0C,
    Float                   = 0x14'06,
    HalfFloat               = 0x14'0B,
    Int                     = 0x14'04,
    Int2101010Rev           = 0x8D'9F,
    Short                   = 0x14'02,
    UnsignedByte            = 0x14'01,
    UnsignedInt             = 0x14'05,
    UnsignedInt10f11f11fRev = 0x8C'3B,
    UnsignedInt2101010Rev   = 0x83'68,
    UnsignedShort           = 0x14'03
};

}  // namespace ogls::oglCore::vertex

#endif
