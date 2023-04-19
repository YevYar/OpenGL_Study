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

void Buffer::unbindTarget(BufferTarget target) noexcept
{
    OGLS_GLCall(glBindBuffer(helpers::toUType(target), 0));
}

Buffer& Buffer::operator=(Buffer&& obj) noexcept
{
    m_impl = std::move(obj.m_impl);
    return *this;
}

void Buffer::bind() const noexcept
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
        OGLS_GLCall(glNamedBufferSubData(m_impl->rendererId, 0, GLsizeiptr{m_impl->data.size}, m_impl->data.pointer));
        m_impl->data = std::move(data);
    }
}

void Buffer::unbind() const noexcept
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

Buffer::Impl::~Impl()
{
    deleteBuffer();
}

void Buffer::Impl::bindToTarget(BufferTarget target, GLuint bufferId) noexcept
{
    OGLS_GLCall(glBindBuffer(helpers::toUType(target), bufferId));
}

BufferBindingTarget Buffer::Impl::getTargetAssociatedGetParameter(BufferTarget target) noexcept
{
    switch (target)
    {
        case BufferTarget::ARRAY_BUFFER:
            return BufferBindingTarget::ARRAY_BUFFER_BINDING;
        case BufferTarget::ATOMIC_COUNTER_BUFFER:
            return BufferBindingTarget::ATOMIC_COUNTER_BUFFER_BINDING;
        case BufferTarget::COPY_READ_BUFFER:
            return BufferBindingTarget::COPY_READ_BUFFER_BINDING;
        case BufferTarget::COPY_WRITE_BUFFER:
            return BufferBindingTarget::COPY_WRITE_BUFFER_BINDING;
        case BufferTarget::DISPATCH_INDIRECT_BUFFER:
            return BufferBindingTarget::DISPATCH_INDIRECT_BUFFER_BINDING;
        case BufferTarget::DRAW_INDIRECT_BUFFER:
            return BufferBindingTarget::DRAW_INDIRECT_BUFFER_BINDING;
        case BufferTarget::ELEMENT_ARRAY_BUFFER:
            return BufferBindingTarget::ELEMENT_ARRAY_BUFFER_BINDING;
        case BufferTarget::PIXEL_PACK_BUFFER:
            return BufferBindingTarget::PIXEL_PACK_BUFFER_BINDING;
        case BufferTarget::PIXEL_UNPACK_BUFFER:
            return BufferBindingTarget::PIXEL_UNPACK_BUFFER_BINDING;
        case BufferTarget::QUERY_BUFFER:
            return BufferBindingTarget::QUERY_BUFFER_BINDING;
        case BufferTarget::SHADER_STORAGE_BUFFER:
            return BufferBindingTarget::SHADER_STORAGE_BUFFER_BINDING;
        case BufferTarget::TEXTURE_BUFFER:
            return BufferBindingTarget::TEXTURE_BUFFER_BINDING;
        case BufferTarget::TRANSFORM_FEEDBACK_BUFFER:
            return BufferBindingTarget::TRANSFORM_FEEDBACK_BUFFER_BINDING;
        case BufferTarget::UNIFORM_BUFFER:
            return BufferBindingTarget::UNIFORM_BUFFER_BINDING;
        default:
        {
            OGLS_ASSERT(false);
            return BufferBindingTarget::ARRAY_BUFFER_BINDING;
        }
    }
}

void Buffer::Impl::bind() const noexcept
{
    Impl::bindToTarget(target, rendererId);
}

bool Buffer::Impl::checkAndGenerateNewStorage(const ArrayData& d) noexcept
{
    if (d.size != data.size)
    {
        OGLS_GLCall(glNamedBufferData(rendererId, GLsizeiptr{d.size}, d.pointer, helpers::toUType(usage)));
        data = d;
        return true;
    }
    return false;
}

void Buffer::Impl::deleteBuffer() noexcept
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
