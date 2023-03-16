#include "texture.h"
#include "textureImpl.h"

#include <glad/glad.h>
#include <stdexcept>

#include "exceptions.h"
#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"
#include "textureTypes.h"

using namespace texture;

BaseTexture::BaseTexture(std::unique_ptr<BaseImpl> impl) : m_impl{ std::move(impl) }
{
}

BaseTexture::BaseTexture(const BaseTexture& obj) :
    m_impl{ std::make_unique<BaseImpl>(*obj.m_impl) }
{
}

BaseTexture::~BaseTexture() = default;

BaseTexture* BaseTexture::clone() const
{
    return new BaseTexture(*this);
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Texture(TextureTarget target) : BaseTexture{ std::make_unique<Texture::Impl>(target) }
{
    if (impl()->m_dimensionsNumber == 1 && target != TextureTarget::TEXTURE_1D)
    {
        throw std::invalid_argument("For DimensionsNumber = 1 only TextureTarget::TEXTURE_1D is supported.");
    }
    if (impl()->m_dimensionsNumber == 2 && target != TextureTarget::TEXTURE_2D)
    {
        throw std::invalid_argument("For DimensionsNumber = 2 only TextureTarget::TEXTURE_2D is supported.");
    }
    if (impl()->m_dimensionsNumber == 3 && target != TextureTarget::TEXTURE_3D)
    {
        throw std::invalid_argument("For DimensionsNumber = 3 only TextureTarget::TEXTURE_3D is supported.");
    }
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Texture(TextureTarget target, TexImageTarget texImageTarget,
    std::shared_ptr<TextureData> textureData) : Texture(target)
{
    setData(texImageTarget, std::move(textureData));
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Texture(const Texture& obj) : BaseTexture{ std::make_unique<Texture::Impl>(*obj.impl()) }
{
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::~Texture() = default;

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::unbindTarget(TextureTarget target) noexcept
{
    GLCall(glBindTexture(helpers::toUType(target), 0));
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>* Texture<DimensionsNumber>::clone() const
{
    return new Texture(*this);
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::bind() const noexcept
{
    impl()->bind();
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::unbind() const noexcept
{
    Texture::unbindTarget(m_impl->m_target);
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::specifyTextureStorageFormat(const std::shared_ptr<TextureData>& textureData) const noexcept
{
    impl()->specifyTextureStorageFormat(textureData);
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::setData(TexImageTarget texImageTarget,
    std::shared_ptr<TextureData> textureData)
{
    impl()->setData(texImageTarget, std::move(textureData));
}

template<unsigned int DimensionsNumber>
TextureTarget texture::Texture<DimensionsNumber>::getTarget() const noexcept
{
    return m_impl->m_target;
}

template<unsigned int DimensionsNumber>
std::shared_ptr<TextureData> Texture<DimensionsNumber>::getData() const noexcept
{
    return impl()->m_data;
}

template<unsigned int DimensionsNumber>
template<typename Type>
requires std::is_same_v<GLfloat, Type> || std::is_same_v<GLint, Type>
void Texture<DimensionsNumber>::setParameter(TexParameterName parameter, Type value)
{
    if constexpr (std::is_same_v<GLfloat, Type>)
    {
        GLCall(glTextureParameterf(impl()->m_rendererId, helpers::toUType(parameter), value));
    }
    else
    {
        GLCall(glTextureParameteri(impl()->m_rendererId, helpers::toUType(parameter), value));
    }
}

template<unsigned int DimensionsNumber>
template<typename Type>
    requires std::is_same_v<GLfloat, Type> || std::is_same_v<GLint, Type>
void Texture<DimensionsNumber>::setParameterV(TexParameterName parameter, const Type* values)
{
    if constexpr (std::is_same_v<GLfloat, Type>)
    {
        GLCall(glTextureParameterfv(impl()->m_rendererId, helpers::toUType(parameter), values));
    }
    else
    {
        GLCall(glTextureParameteriv(impl()->m_rendererId, helpers::toUType(parameter), values));
    }
}

template<unsigned int DimensionsNumber>
template<typename Type>
    requires std::is_same_v<GLint, Type> || std::is_same_v<GLuint, Type>
void Texture<DimensionsNumber>::setParameterIV(TexParameterName parameter, const Type* values)
{
    if constexpr (std::is_same_v<GLint, Type>)
    {
        GLCall(glTextureParameterIiv(impl()->m_rendererId, helpers::toUType(parameter), values));
    }
    else
    {
        GLCall(glTextureParameterIuiv(impl()->m_rendererId, helpers::toUType(parameter), values));
    }
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Impl* Texture<DimensionsNumber>::impl() const noexcept
{
    return static_cast<Texture<DimensionsNumber>::Impl*>(m_impl.get());
}


// IMPLEMENTATION

BaseTexture::BaseImpl::BaseImpl(TextureTarget target) : m_target{ target }
{
    genTexture();
}

BaseTexture::BaseImpl::BaseImpl(const BaseImpl& obj) : m_target{ obj.m_target }
{
    genTexture();
}

BaseTexture::BaseImpl::~BaseImpl()
{
    deleteTexture();
}

void BaseTexture::BaseImpl::genTexture()
{
    GLCall(glCreateTextures(helpers::toUType(m_target), 1, &m_rendererId));
    if (m_rendererId == 0)
    {
        throw exceptions::GLRecAcquisitionException("Texture cannot be generated.");
    }
}

void BaseTexture::BaseImpl::deleteTexture() noexcept
{
    GLCall(glDeleteTextures(1, &m_rendererId));
    m_rendererId = 0;
}

void TexDimensionSpecificFunc<1>::setTexStorageFormat(GLuint textureId,
    const std::shared_ptr<TextureData>&textureData) const noexcept
{
    GLCall(glTextureStorage1D(textureId, textureData->m_level, helpers::toUType(textureData->m_internalFormat),
        textureData->m_width));
}

void TexDimensionSpecificFunc<1>::setTexImageInTarget(GLuint textureId, TexImageTarget target,
    std::shared_ptr<TextureData> textureData)
{
    using namespace helpers;
    
    GLCall(glTextureSubImage1D(textureId, 0, 0, textureData->m_width, toUType(textureData->m_format),
        toUType(textureData->m_type), textureData->m_data));
}

void TexDimensionSpecificFunc<2>::setTexStorageFormat(GLuint textureId,
    const std::shared_ptr<TextureData>& textureData) const noexcept
{
    GLCall(glTextureStorage2D(textureId, textureData->m_level, helpers::toUType(textureData->m_internalFormat),
        textureData->m_width, textureData->m_height));
}

void TexDimensionSpecificFunc<2>::setTexImageInTarget(GLuint textureId, TexImageTarget target,
    std::shared_ptr<TextureData> textureData)
{
    using namespace helpers;
    
    GLCall(glTextureSubImage2D(textureId, 0, 0, 0, textureData->m_width, textureData->m_height,
        toUType(textureData->m_format), toUType(textureData->m_type), textureData->m_data));
}

void TexDimensionSpecificFunc<3>::setTexStorageFormat(GLuint textureId,
    const std::shared_ptr<TextureData>& textureData) const noexcept
{
    GLCall(glTextureStorage3D(textureId, textureData->m_level, helpers::toUType(textureData->m_internalFormat),
        textureData->m_width, textureData->m_height, textureData->m_depth));
}

void TexDimensionSpecificFunc<3>::setTexImageInTarget(GLuint textureId, TexImageTarget target,
    std::shared_ptr<TextureData> textureData)
{
    using namespace helpers;
    
    GLCall(glTextureSubImage3D(textureId, 0, 0, 0, 0, textureData->m_width, textureData->m_height,
        textureData->m_depth, toUType(textureData->m_format), toUType(textureData->m_type),textureData->m_data));
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Impl::Impl(TextureTarget target) : BaseImpl(target)
{
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Impl::Impl(const Impl& obj) : BaseImpl(obj)
{
    m_lastTexImageTarget = obj.m_lastTexImageTarget;
    setData(m_lastTexImageTarget, obj.m_data);
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::Impl::bindToTarget(TextureTarget target, GLuint textureId) noexcept
{
    GLCall(glBindTexture(helpers::toUType(target), textureId));
}

template<unsigned int DimensionsNumber>
TextureBindingTarget Texture<DimensionsNumber>::Impl::getTargetAssociatedGetParameter(TextureTarget target) noexcept
{
    switch (target)
    {
        case TextureTarget::TEXTURE_1D:
            return TextureBindingTarget::TEXTURE_BINDING_1D;
        case TextureTarget::TEXTURE_1D_ARRAY:
            return TextureBindingTarget::TEXTURE_BINDING_1D_ARRAY;
        case TextureTarget::TEXTURE_2D:
            return TextureBindingTarget::TEXTURE_BINDING_2D;
        case TextureTarget::TEXTURE_2D_ARRAY:
            return TextureBindingTarget::TEXTURE_BINDING_2D_ARRAY;
        case TextureTarget::TEXTURE_2D_MULTISAMPLE:
            return TextureBindingTarget::TEXTURE_BINDING_2D_MULTISAMPLE;
        case TextureTarget::TEXTURE_2D_MULTISAMPLE_ARRAY:
            return TextureBindingTarget::TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY;
        case TextureTarget::TEXTURE_3D:
            return TextureBindingTarget::TEXTURE_BINDING_3D;
        case TextureTarget::TEXTURE_BUFFER:
            return TextureBindingTarget::TEXTURE_BINDING_BUFFER;
        case TextureTarget::TEXTURE_CUBE_MAP:
            return TextureBindingTarget::TEXTURE_BINDING_CUBE_MAP;
        case TextureTarget::TEXTURE_CUBE_MAP_ARRAY:
            return TextureBindingTarget::TEXTURE_BINDING_CUBE_MAP_ARRAY;
        case TextureTarget::TEXTURE_RECTANGLE:
            return TextureBindingTarget::TEXTURE_BINDING_RECTANGLE;
    }
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::Impl::bind() const noexcept
{
    Impl::bindToTarget(m_target, m_rendererId);
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::Impl::
    specifyTextureStorageFormat(const std::shared_ptr<TextureData>& textureData) const noexcept
{
    ASSERT(!m_isStorageFormatSpecified);
    m_specific.setTexStorageFormat(m_rendererId, textureData);
    m_isStorageFormatSpecified = true;
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::Impl::setData(TexImageTarget texImageTarget,
    std::shared_ptr<TextureData> textureData)
{
    if (!m_isStorageFormatSpecified)
    {
        specifyTextureStorageFormat(textureData);
    }

    m_specific.setTexImageInTarget(m_rendererId, texImageTarget, textureData);
    GLCall(glGenerateTextureMipmap(m_rendererId));

    m_data = std::move(textureData);
    m_lastTexImageTarget = texImageTarget;
}

#define INSTANTIATE_TEXTURE(N) \
    template class Texture<N>;\
    template void Texture<N>::setParameter(TexParameterName, GLfloat);\
    template void Texture<N>::setParameter(TexParameterName, GLint);\
    template void Texture<N>::setParameterV(TexParameterName, const GLfloat*);\
    template void Texture<N>::setParameterV(TexParameterName, const GLint*);\
    template void Texture<N>::setParameterIV(TexParameterName, const GLint*);\
    template void Texture<N>::setParameterIV(TexParameterName, const GLuint*);\

INSTANTIATE_TEXTURE(1)
INSTANTIATE_TEXTURE(2)
INSTANTIATE_TEXTURE(3)
