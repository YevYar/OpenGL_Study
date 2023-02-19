#include "texture.h"

#include <glad/glad.h>

#include "helpers/helpers.h"
#include "helpers/debugHelpers.h"

using namespace texture;

Texture::Texture(TextureBindingTarget target, std::shared_ptr<TextureData> textureData) : m_target{ target },
    m_data{ std::move(textureData) }
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
