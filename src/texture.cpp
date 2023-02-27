#include "texture.h"

#include <format>
#include <glad/glad.h>
#include <map>
#include <stdexcept>

#include "exceptions.h"
#include "openglLimits.h"

using namespace texture;

namespace
{
    /**
     * \brief Texture unit ID - { Texture target, Texture ID }
     */
    std::map<GLuint, std::map<TextureTarget, GLuint>> textureUnits;

    TextureBindingTarget getTargetAssociatedGetParameter(TextureTarget target) noexcept;
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Texture(TextureTarget target) :
    m_target{ target }
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
    bindForAMomentAndExecute();
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Texture(TextureTarget target, TexImageTarget texImageTarget,
    std::shared_ptr<TextureData> textureData) : Texture(target)
{
    setData(texImageTarget, std::move(textureData));
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Texture(const Texture& obj) : m_target{ obj.m_target },
    m_lastTexImageTarget{ obj.m_lastTexImageTarget }
{
    genTexture();
    setData(m_lastTexImageTarget, obj.m_data);
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Texture(Texture&& obj) noexcept : m_rendererId{ obj.m_rendererId },
    m_target{ obj.m_target }, m_data{ std::move(obj.m_data) }, m_lastTexImageTarget{ obj.m_lastTexImageTarget },
    m_isBound{ obj.m_isBound }
{
    obj.m_rendererId = 0;
    obj.m_isBound = false;
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
    m_isBound = obj.m_isBound;

    obj.m_rendererId = 0;
    obj.m_isBound = false;

    return *this;
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::bindTextureToTextureUnit(GLuint textureUnitId, TextureTarget textureTarget,
    const Texture<DimensionsNumber>& textureObj)
{
    const auto maxTUnitIndex = getOpenglLimit(LimitName::MAX_COMBINED_TEXTURE_IMAGE_UNITS);
    if (textureUnitId > maxTUnitIndex - 1)
    {
        const auto errorMessage = std::format("Texture unit index must be less than {}.", maxTUnitIndex);
        throw std::out_of_range(errorMessage);
    }

    const auto textureUnit = textureUnits.find(textureUnitId);
    if (textureUnit != textureUnits.end())
    {
        const auto texture = textureUnit->second.find(textureTarget);
        if (texture != textureUnit->second.end() && texture->second == textureObj.m_rendererId)
        {
            return;
        }

        textureUnit->second.insert_or_assign(textureTarget, textureObj.m_rendererId);
        glBindTextureUnit(textureUnitId, textureObj.m_rendererId);
    }
    else
    {
        textureUnits.insert(
            { textureUnitId, std::map<TextureTarget, GLuint>({ { textureTarget, textureObj.m_rendererId } }) });
        glBindTextureUnit(textureUnitId, textureObj.m_rendererId);
    }
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::unbindTarget(TextureTarget target) noexcept
{
    GLCall(glBindTexture(helpers::toUType(target), 0));
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::bind() const noexcept
{
    GLCall(glBindTexture(helpers::toUType(m_target), m_rendererId));
    m_isBound = true;
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::unbind() const noexcept
{
    if (m_isBound)
    {
        Texture::unbindTarget(m_target);
        m_isBound = false;
    }    
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::setData(TexImageTarget texImageTarget,
    std::shared_ptr<TextureData> textureData)
{
    bindForAMomentAndExecute([&]() {
        m_dimensionTypesAndFunc.setTexImageInTarget(texImageTarget, textureData);
    });  

    m_data = std::move(textureData);
    m_lastTexImageTarget = texImageTarget;
}

template<unsigned int DimensionsNumber>
std::shared_ptr<TextureData> Texture<DimensionsNumber>::getData() const noexcept
{
    return m_data;
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::genTexture()
{
    GLCall(glGenTextures(1, &m_rendererId));
    if (m_rendererId == 0)
    {
        throw exceptions::GLRecAcquisitionException("Texture cannot be generated.");
    }
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::bindForAMomentAndExecute(const std::function<void()>& funcToExecute)
{
    using namespace helpers;

    GLuint boundTexture = 0;
    GLCall(glGetIntegerv(toUType(getTargetAssociatedGetParameter(m_target)),
        reinterpret_cast<GLint*>(&boundTexture)));
    GLCall(glBindTexture(toUType(m_target), m_rendererId));

    funcToExecute();

    GLCall(glBindTexture(toUType(m_target), boundTexture));
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::deleteTexture() noexcept
{
    GLCall(glDeleteTextures(1, &m_rendererId));
    m_rendererId = 0;
}

namespace
{
    TextureBindingTarget getTargetAssociatedGetParameter(TextureTarget target) noexcept
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
}

template class Texture<1>;
template class Texture<2>;
template class Texture<3>;
