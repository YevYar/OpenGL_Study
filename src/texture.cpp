#include "texture.h"

#include <glad/glad.h>

#include "exceptions.h"
#include "helpers/helpers.h"
#include "helpers/debugHelpers.h"

using namespace texture;

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Texture(TextureBindingTarget target, std::shared_ptr<TextureData> textureData) : m_target{ target }
{
    genTexture();
    // setData(std::move(textureData));
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Texture(const Texture& obj) : m_target{ obj.m_target }
{
    genTexture();
    // setData(typesAndFunc. obj.m_data);
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::Texture(Texture&& obj) noexcept : m_rendererId{ obj.m_rendererId }, m_target{ obj.m_target },
    m_data{ std::move(obj.m_data) }, m_isDataSet{ obj.m_isDataSet }
{
    obj.m_rendererId = 0;
    obj.m_isDataSet = true;
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>::~Texture()
{
    GLCall(glDeleteTextures(1, &m_rendererId));
}

template<unsigned int DimensionsNumber>
Texture<DimensionsNumber>& Texture<DimensionsNumber>::operator=(const Texture& obj)
{
   // m_
    return *this;
}

// Texture::Texture(TextureTarget target, std::shared_ptr<TextureData> textureData)

//template<typename>
//void Texture<DimensionsNumber>::setData(std::shared_ptr<TextureData> textureData)
//{
//
//}

template<unsigned int DimensionsNumber>
void Texture<DimensionsNumber>::genTexture()
{
    GLCall(glGenTextures(1, &m_rendererId));
    if (m_rendererId == 0)
    {
        throw exceptions::GLRecAcquisitionException("Texture cannot be generated.");
    }
}

template class Texture<1>;
template class Texture<2>;
template class Texture<3>;
