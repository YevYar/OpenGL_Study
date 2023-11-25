#include "texture.h"
#include "textureImpl.h"

#include <stdexcept>

#include "exceptions.h"
#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"

namespace ogls::oglCore::texture
{
BaseTexture::BaseTexture(std::unique_ptr<BaseImpl> impl) noexcept : m_impl{std::move(impl)}
{
}

BaseTexture::BaseTexture(const BaseTexture& obj) : m_impl{std::make_unique<BaseImpl>(*obj.m_impl)}
{
}

BaseTexture::~BaseTexture() noexcept = default;

BaseTexture* BaseTexture::clone() const
{
    return new BaseTexture{*this};
}

template<size_t DimensionsNumber>
Texture<DimensionsNumber>::Texture(TextureTarget target) : BaseTexture{std::make_unique<Texture::Impl>(target)}
{
    if (impl()->dimensionsNumber == 1 && target != TextureTarget::Texture1d)
    {
        throw std::invalid_argument{"For DimensionsNumber = 1 only TextureTarget::Texture1d is supported."};
    }
    if (impl()->dimensionsNumber == 2 && target != TextureTarget::Texture2d)
    {
        throw std::invalid_argument{"For DimensionsNumber = 2 only TextureTarget::Texture2d is supported."};
    }
    if (impl()->dimensionsNumber == 3 && target != TextureTarget::Texture3d)
    {
        throw std::invalid_argument{"For DimensionsNumber = 3 only TextureTarget::Texture3d is supported."};
    }
}

template<size_t DimensionsNumber>
Texture<DimensionsNumber>::Texture(TextureTarget target, std::shared_ptr<TextureData> textureData) : Texture{target}
{
    setData(std::move(textureData));
}

template<size_t DimensionsNumber>
Texture<DimensionsNumber>::Texture(const Texture& obj) : BaseTexture{std::make_unique<Texture::Impl>(*obj.impl())}
{
}

template<size_t DimensionsNumber>
Texture<DimensionsNumber>::~Texture() noexcept = default;

template<size_t DimensionsNumber>
void Texture<DimensionsNumber>::unbindTarget(TextureTarget target)
{
    OGLS_GLCall(glBindTexture(helpers::toUType(target), 0));
}

template<size_t DimensionsNumber>
void Texture<DimensionsNumber>::bind() const
{
    impl()->bind();
}

template<size_t DimensionsNumber>
std::shared_ptr<TextureData> Texture<DimensionsNumber>::getData() const noexcept
{
    return impl()->data;
}

template<size_t DimensionsNumber>
TextureTarget Texture<DimensionsNumber>::getTarget() const noexcept
{
    return m_impl->target;
}

template<size_t DimensionsNumber>
void Texture<DimensionsNumber>::setData(std::shared_ptr<TextureData> textureData)
{
    impl()->setData(std::move(textureData));
}

template<size_t DimensionsNumber>
template<typename Type>
requires std::is_same_v<GLfloat, Type> || std::is_same_v<GLint, Type>
void Texture<DimensionsNumber>::setParameter(TexParameterName parameter, Type value)
{
    if constexpr (std::is_same_v<GLfloat, Type>)
    {
        OGLS_GLCall(glTextureParameterf(impl()->rendererId, helpers::toUType(parameter), value));
    }
    else
    {
        OGLS_GLCall(glTextureParameteri(impl()->rendererId, helpers::toUType(parameter), value));
    }
}

template<size_t DimensionsNumber>
template<typename Type>
requires std::is_same_v<GLint, Type> || std::is_same_v<GLuint, Type>
void Texture<DimensionsNumber>::setParameterIV(TexParameterName parameter, const Type* values)
{
    if constexpr (std::is_same_v<GLint, Type>)
    {
        OGLS_GLCall(glTextureParameterIiv(impl()->rendererId, helpers::toUType(parameter), values));
    }
    else
    {
        OGLS_GLCall(glTextureParameterIuiv(impl()->rendererId, helpers::toUType(parameter), values));
    }
}

template<size_t DimensionsNumber>
template<typename Type>
requires std::is_same_v<GLfloat, Type> || std::is_same_v<GLint, Type>
void Texture<DimensionsNumber>::setParameterV(TexParameterName parameter, const Type* values)
{
    if constexpr (std::is_same_v<GLfloat, Type>)
    {
        OGLS_GLCall(glTextureParameterfv(impl()->rendererId, helpers::toUType(parameter), values));
    }
    else
    {
        OGLS_GLCall(glTextureParameteriv(impl()->rendererId, helpers::toUType(parameter), values));
    }
}

template<size_t DimensionsNumber>
void Texture<DimensionsNumber>::specifyTextureStorageFormat(const std::shared_ptr<TextureData>& textureData)
{
    impl()->specifyTextureStorageFormat(textureData);
}

template<size_t DimensionsNumber>
void Texture<DimensionsNumber>::unbind() const
{
    Texture::unbindTarget(m_impl->target);
}

template<size_t DimensionsNumber>
Texture<DimensionsNumber>* Texture<DimensionsNumber>::clone() const
{
    return new Texture{*this};
}

template<size_t DimensionsNumber>
typename Texture<DimensionsNumber>::Impl* Texture<DimensionsNumber>::impl() const noexcept
{
    return static_cast<Texture<DimensionsNumber>::Impl*>(m_impl.get());
}

//------ IMPLEMENTATION

BaseTexture::BaseImpl::BaseImpl(TextureTarget t) : target{t}
{
    genTexture();
}

BaseTexture::BaseImpl::BaseImpl(const BaseImpl& obj) : target{obj.target}
{
    genTexture();
}

BaseTexture::BaseImpl::~BaseImpl() noexcept
{
    try
    {
        deleteTexture();
    }
    catch (...)
    {
    }
}

void BaseTexture::BaseImpl::deleteTexture()
{
    OGLS_GLCall(glDeleteTextures(1, &rendererId));
    rendererId = {0};
}

void BaseTexture::BaseImpl::genTexture()
{
    OGLS_GLCall(glCreateTextures(helpers::toUType(target), 1, &rendererId));
    if (rendererId == 0)
    {
        throw exceptions::GLRecAcquisitionException{"Texture cannot be generated."};
    }
}

void TexDimensionSpecificFunc<1>::setTexImageInTarget(GLuint textureId, std::shared_ptr<TextureData> textureData)
{
    using namespace helpers;


    OGLS_GLCall(glTextureSubImage1D(textureId, 0, 0, textureData->width, toUType(textureData->format),
                                    toUType(textureData->type), textureData->data));
}

void TexDimensionSpecificFunc<1>::setTexStorageFormat(GLuint textureId, const std::shared_ptr<TextureData>& textureData)
{
    OGLS_GLCall(glTextureStorage1D(textureId, textureData->level, helpers::toUType(textureData->internalFormat),
                                   textureData->width));
}

void TexDimensionSpecificFunc<2>::setTexImageInTarget(GLuint textureId, std::shared_ptr<TextureData> textureData)
{
    using namespace helpers;


    OGLS_GLCall(glTextureSubImage2D(textureId, 0, 0, 0, textureData->width, textureData->height,
                                    toUType(textureData->format), toUType(textureData->type), textureData->data));
}

void TexDimensionSpecificFunc<2>::setTexStorageFormat(GLuint textureId, const std::shared_ptr<TextureData>& textureData)
{
    OGLS_GLCall(glTextureStorage2D(textureId, textureData->level, helpers::toUType(textureData->internalFormat),
                                   textureData->width, textureData->height));
}

void TexDimensionSpecificFunc<3>::setTexImageInTarget(GLuint textureId, std::shared_ptr<TextureData> textureData)
{
    using namespace helpers;


    OGLS_GLCall(glTextureSubImage3D(textureId, 0, 0, 0, 0, textureData->width, textureData->height, textureData->depth,
                                    toUType(textureData->format), toUType(textureData->type), textureData->data));
}

void TexDimensionSpecificFunc<3>::setTexStorageFormat(GLuint textureId, const std::shared_ptr<TextureData>& textureData)
{
    OGLS_GLCall(glTextureStorage3D(textureId, textureData->level, helpers::toUType(textureData->internalFormat),
                                   textureData->width, textureData->height, textureData->depth));
}

template<size_t DimensionsNumber>
Texture<DimensionsNumber>::Impl::Impl(TextureTarget t) : BaseImpl{t}
{
}

template<size_t DimensionsNumber>
Texture<DimensionsNumber>::Impl::Impl(const Impl& obj) : BaseImpl{obj}
{
    setData(obj.data);
}

template<size_t DimensionsNumber>
void Texture<DimensionsNumber>::Impl::bindToTarget(TextureTarget target, GLuint textureId)
{
    OGLS_GLCall(glBindTexture(helpers::toUType(target), textureId));
}

template<size_t DimensionsNumber>
TextureBindingTarget Texture<DimensionsNumber>::Impl::getTargetAssociatedGetParameter(TextureTarget target) noexcept
{
    switch (target)
    {
        case TextureTarget::Texture1d:
            return TextureBindingTarget::TextureBinding1d;
        case TextureTarget::Texture1dArray:
            return TextureBindingTarget::TextureBinding1dArray;
        case TextureTarget::Texture2d:
            return TextureBindingTarget::TextureBinding2d;
        case TextureTarget::Texture2dArray:
            return TextureBindingTarget::TextureBinding2dArray;
        case TextureTarget::Texture2dMultisample:
            return TextureBindingTarget::TextureBinding2dMultisample;
        case TextureTarget::Texture2dMultisampleArray:
            return TextureBindingTarget::TextureBinding2dMultisampleArray;
        case TextureTarget::Texture3d:
            return TextureBindingTarget::TextureBinding3d;
        case TextureTarget::TextureBuffer:
            return TextureBindingTarget::TextureBindingBuffer;
        case TextureTarget::TextureCubeMap:
            return TextureBindingTarget::TextureBindingCubeMap;
        case TextureTarget::TextureCubeMapArray:
            return TextureBindingTarget::TextureBindingCubeMapArray;
        case TextureTarget::TextureRectangle:
            return TextureBindingTarget::TextureBindingRectangle;
        default:
        {
            OGLS_ASSERT(false);
            return TextureBindingTarget::TextureBinding2d;  // corresponds to default value of BaseImpl::target
        }
    }
}

template<size_t DimensionsNumber>
void Texture<DimensionsNumber>::Impl::bind() const
{
    Impl::bindToTarget(target, rendererId);
}

template<size_t DimensionsNumber>
void Texture<DimensionsNumber>::Impl::setData(std::shared_ptr<TextureData> textureData)
{
    if (!isStorageFormatSpecified)
    {
        specifyTextureStorageFormat(textureData);
    }

    specific.setTexImageInTarget(rendererId, textureData);
    OGLS_GLCall(glGenerateTextureMipmap(rendererId));

    data = std::move(textureData);
}

template<size_t DimensionsNumber>
void Texture<DimensionsNumber>::Impl::specifyTextureStorageFormat(const std::shared_ptr<TextureData>& textureData)
{
    OGLS_ASSERT(!isStorageFormatSpecified);
    specific.setTexStorageFormat(rendererId, textureData);
    isStorageFormatSpecified = true;
}

#define INSTANTIATE_TEXTURE(N)                                                 \
    template class Texture<N>;                                                 \
    template void Texture<N>::setParameter(TexParameterName, GLfloat);         \
    template void Texture<N>::setParameter(TexParameterName, GLint);           \
    template void Texture<N>::setParameterV(TexParameterName, const GLfloat*); \
    template void Texture<N>::setParameterV(TexParameterName, const GLint*);   \
    template void Texture<N>::setParameterIV(TexParameterName, const GLint*);  \
    template void Texture<N>::setParameterIV(TexParameterName, const GLuint*);

INSTANTIATE_TEXTURE(1);
INSTANTIATE_TEXTURE(2);
INSTANTIATE_TEXTURE(3);

#undef INSTANTIATE_TEXTURE

}  // namespace ogls::oglCore::texture
