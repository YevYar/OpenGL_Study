#include "texture.h"

#include <glad/glad.h>
#include <stdexcept>

#include "debugHelpers.h"
#include "exceptions.h"
#include "textureImpl.h"

using namespace texture;

BaseTexture::BaseTexture(TextureTarget target) : m_target{ target }
{
}

BaseTexture::BaseTexture(GLuint rendererId, TextureTarget target) : m_rendererId{ rendererId }, m_target{ target }
{
}

BaseTexture* BaseTexture::clone() const
{
    return new BaseTexture(this->m_rendererId, this->m_target);
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Texture(TextureTarget target) : BaseTexture{ target }
{
    if (m_dimensionsNumber == 1 && target != TextureTarget::TEXTURE_1D)
    {
        throw std::invalid_argument("For DimensionsNumber = 1 only TextureTarget::TEXTURE_1D is supported.");
    }
    if (m_dimensionsNumber == 2 && target != TextureTarget::TEXTURE_2D)
    {
        throw std::invalid_argument("For DimensionsNumber = 2 only TextureTarget::TEXTURE_2D is supported.");
    }
    if (m_dimensionsNumber == 3 && target != TextureTarget::TEXTURE_3D)
    {
        throw std::invalid_argument("For DimensionsNumber = 3 only TextureTarget::TEXTURE_3D is supported.");
    }

    genTexture();
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Texture(TextureTarget target, TexImageTarget texImageTarget,
    std::shared_ptr<TextureData> textureData) : Texture(target)
{
    setData(texImageTarget, std::move(textureData));
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Texture(const Texture& obj) : BaseTexture{ obj.m_target },
    m_lastTexImageTarget{ obj.m_lastTexImageTarget }
{
    genTexture();
    setData(m_lastTexImageTarget, obj.m_data);
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Texture(Texture&& obj) noexcept : BaseTexture{ obj.m_rendererId, obj.m_target },
    m_data{ std::move(obj.m_data) }, m_lastTexImageTarget{ obj.m_lastTexImageTarget },
    m_isStorageFormatSpecified{ obj.m_isStorageFormatSpecified }
{
    obj.m_rendererId = 0;
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::~Texture()
{
    deleteTexture();
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>& Texture<DimensionsNumber>::operator=(Texture&& obj) noexcept
{
    deleteTexture();

    m_rendererId = obj.m_rendererId;
    m_target = obj.m_target;
    m_data = std::move(obj.m_data);
    m_lastTexImageTarget = obj.m_lastTexImageTarget;
    m_isStorageFormatSpecified = obj.m_isStorageFormatSpecified;

    obj.m_rendererId = 0;

    return *this;
}

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
    Texture::bindToTarget(m_target, m_rendererId);
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::unbind() const noexcept
{
    Texture::unbindTarget(m_target);
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::specifyTextureStorageFormat(const std::shared_ptr<TextureData>& textureData) const noexcept
{
    ASSERT(!m_isStorageFormatSpecified);

    m_dimensionTypesAndFunc->setTexStorageFormat(m_rendererId, textureData);
    m_isStorageFormatSpecified = true;
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::setData(TexImageTarget texImageTarget,
    std::shared_ptr<TextureData> textureData)
{
    if (!m_isStorageFormatSpecified)
    {
        specifyTextureStorageFormat(textureData);
    }

    //m_dimensionTypesAndFunc->setTexImageInTarget(m_rendererId, texImageTarget, textureData);
    GLCall(glGenerateTextureMipmap(m_rendererId));

    m_data = std::move(textureData);
    m_lastTexImageTarget = texImageTarget;
}

template<unsigned int DimensionsNumber>
TextureTarget texture::Texture<DimensionsNumber>::getTarget() const noexcept
{
    return m_target;
}

template<unsigned int DimensionsNumber>
std::shared_ptr<TextureData> Texture<DimensionsNumber>::getData() const noexcept
{
    return m_data;
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::bindToTarget(TextureTarget target, GLuint textureId) noexcept
{
    GLCall(glBindTexture(helpers::toUType(target), textureId));
}

template<unsigned int DimensionsNumber>
TextureBindingTarget Texture<DimensionsNumber>::getTargetAssociatedGetParameter(TextureTarget target) noexcept
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
void Texture<DimensionsNumber>::genTexture()
{
    GLCall(glCreateTextures(helpers::toUType(m_target), 1, &m_rendererId));
    if (m_rendererId == 0)
    {
        throw exceptions::GLRecAcquisitionException("Texture cannot be generated.");
    }
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::deleteTexture() noexcept
{
    GLCall(glDeleteTextures(1, &m_rendererId));
    m_rendererId = 0;
}

template class Texture<1>;
template class Texture<2>;
template class Texture<3>;
