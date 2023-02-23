#include "texture.h"

#include <glad/glad.h>
#include <stdexcept>

#include "exceptions.h"

using namespace texture;

namespace
{
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
    bind();
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
    m_target{ obj.m_target }, m_data{ std::move(obj.m_data) }, m_lastTexImageTarget{ obj.m_lastTexImageTarget }
{
    obj.m_rendererId = 0;
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::~Texture()
{
    GLCall(glDeleteTextures(1, &m_rendererId));
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>& Texture<DimensionsNumber>::operator=(const Texture& obj)
{
    m_target = obj.m_target;
    m_data = obj.m_data;
    m_lastTexImageTarget = obj.m_lastTexImageTarget;



    return *this;
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::unbindTarget(TextureTarget target) noexcept
{
    GLCall(glBindTexture(helpers::toUType(m_target), 0));
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::bind() const noexcept
{
    using namespace helpers;

    GLuint boundTexture = 0;
    GLCall(glGetIntegerv(toUType(getTargetAssociatedGetParameter(m_target)),
        reinterpret_cast<GLint*>(&boundTexture)));
    GLCall(glBindTexture(toUType(m_target), m_rendererId));
    GLCall(glBindTexture(toUType(m_target), boundTexture));
}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::unbind() const noexcept
{
    Texture::unbindTarget(m_target);
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
