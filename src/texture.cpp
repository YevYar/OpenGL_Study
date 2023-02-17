#include "texture.h"

#include <glad/glad.h>

#include "helpers/helpers.h"
#include "helpers/debugHelpers.h"

using namespace texture;

TextureData::TextureData(unsigned char* textureData, int width, int height, int nChannels) :
    m_data{ textureData }, m_height{ height }, m_width{ width }, m_nChannels{ nChannels }
{
    ASSERT(m_data);
    ASSERT(m_height > 0);
    ASSERT(m_width > 0);
    ASSERT(m_nChannels > 0);
}

TextureData::~TextureData()
{
    helpers::freeTextureData(*this);
}

unsigned char* TextureData::getData() const noexcept
{
    return m_data;
}

Texture::Texture(TextureTarget target) : m_target{ target }
{

}

// Texture::Texture(TextureTarget target, std::shared_ptr<TextureData> textureData)
