#include "buffer.h"
#include "bufferImpl.h"

#include "exceptions.h"
#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"
#include "vertexBufferLayoutImpl.h"

namespace ogls::oglCore::vertex
{
Buffer::Buffer(BufferTarget target, ArrayData data, BufferDataUsage usage,
               std::optional<VertexBufferLayout> bufferLayout) :
    m_impl{std::make_unique<Impl>(target, std::move(data), usage, std::move(bufferLayout))}
{
    setData(std::move(m_impl->data));
}

Buffer::Buffer(const Buffer& obj) : m_impl{std::make_unique<Impl>(*obj.m_impl.get())}
{
    setData(std::move(m_impl->data));
}

Buffer::Buffer(Buffer&& obj) noexcept : m_impl{std::move(obj.m_impl)}
{
}

Buffer::~Buffer() = default;

void Buffer::unbindTarget(BufferTarget target)
{
    OGLS_GLCall(glBindBuffer(helpers::toUType(target), 0));
}

Buffer& Buffer::operator=(Buffer&& obj) noexcept
{
    m_impl = std::move(obj.m_impl);
    return *this;
}

void Buffer::bind() const
{
    m_impl->bind();
}

const ArrayData& Buffer::getData() const noexcept
{
    return m_impl->data;
}

std::optional<VertexBufferLayout> Buffer::getLayout() const noexcept
{
    return m_impl->layout;
}

void Buffer::setData(ArrayData data)
{
    if (!m_impl->checkAndGenerateNewStorage(data))
    {
        OGLS_GLCall(glNamedBufferSubData(m_impl->rendererId, 0, m_impl->data.size, m_impl->data.pointer));
        m_impl->data = std::move(data);
    }
}

void Buffer::unbind() const
{
    Buffer::unbindTarget(m_impl->target);
}

Buffer* Buffer::clone() const
{
    return new Buffer{*this};
}

//------ IMPLEMENTATION

Buffer::Impl::Impl(BufferTarget t, ArrayData d, BufferDataUsage u, std::optional<VertexBufferLayout> bL) :
    target{t}, data{std::move(d)}, usage{u}, layout{std::move(bL)}
{
    genBuffer();
}

Buffer::Impl::Impl(const Impl& obj) : target{obj.target}, data{obj.data}, usage{obj.usage}, layout{obj.layout}
{
    genBuffer();
}

Buffer::Impl::Impl(Impl&& obj) noexcept :
    rendererId{obj.rendererId}, target{obj.target}, data{std::move(obj.data)}, usage{obj.usage},
    layout{std::move(obj.layout)}
{
    obj.rendererId = {0};
}

Buffer::Impl::~Impl() noexcept
{
    try
    {
        deleteBuffer();
    }
    catch (...)
    {
    }    
}

void Buffer::Impl::bindToTarget(BufferTarget target, GLuint bufferId)
{
    OGLS_GLCall(glBindBuffer(helpers::toUType(target), bufferId));
}

BufferBindingTarget Buffer::Impl::getTargetAssociatedGetParameter(BufferTarget target) noexcept
{
    switch (target)
    {
        case BufferTarget::ArrayBuffer:
            return BufferBindingTarget::ArrayBufferBinding;
        case BufferTarget::AtomicCounterBuffer:
            return BufferBindingTarget::AtomicCounterBufferBinding;
        case BufferTarget::CopyReadBuffer:
            return BufferBindingTarget::CopyReadBufferBinding;
        case BufferTarget::CopyWriteBuffer:
            return BufferBindingTarget::CopyWriteBufferBinding;
        case BufferTarget::DispatchIndirectBuffer:
            return BufferBindingTarget::DispatchIndirectBufferBinding;
        case BufferTarget::DrawIndirectBuffer:
            return BufferBindingTarget::DrawIndirectBufferBinding;
        case BufferTarget::ElementArrayBuffer:
            return BufferBindingTarget::ElementArrayBufferBinding;
        case BufferTarget::PixelPackBuffer:
            return BufferBindingTarget::PixelPackBufferBinding;
        case BufferTarget::PixelUnpackBuffer:
            return BufferBindingTarget::PixelUnpackBufferBinding;
        case BufferTarget::QueryBuffer:
            return BufferBindingTarget::QueryBufferBinding;
        case BufferTarget::ShaderStorageBuffer:
            return BufferBindingTarget::ShaderStorageBufferBinding;
        case BufferTarget::TextureBuffer:
            return BufferBindingTarget::TextureBufferBinding;
        case BufferTarget::TransformFeedbackBuffer:
            return BufferBindingTarget::TransformFeedbackBufferBinding;
        case BufferTarget::UniformBuffer:
            return BufferBindingTarget::UniformBufferBinding;
        default:
        {
            OGLS_ASSERT(false);
            return BufferBindingTarget::ArrayBufferBinding;
        }
    }
}

void Buffer::Impl::bind() const
{
    Impl::bindToTarget(target, rendererId);
}

bool Buffer::Impl::checkAndGenerateNewStorage(const ArrayData& d)
{
    if (d.size != data.size)
    {
        OGLS_GLCall(glNamedBufferData(rendererId, d.size, d.pointer, helpers::toUType(usage)));
        data = d;
        return true;
    }
    return false;
}

void Buffer::Impl::deleteBuffer()
{
    OGLS_GLCall(glDeleteBuffers(1, &rendererId));
    rendererId = {0};
}

void Buffer::Impl::genBuffer()
{
    OGLS_GLCall(glCreateBuffers(1, &rendererId));
    if (rendererId == 0)
    {
        throw exceptions::GLRecAcquisitionException{"Buffer cannot be generated."};
    }
}

}  // namespace ogls::oglCore::vertex
