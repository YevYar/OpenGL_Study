#include "textureTypes.h"

#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"

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

int TextureData::getHeight() const noexcept
{
    return m_height;
}

int TextureData::getWidth() const noexcept
{
    return m_width;
}

int TextureData::getNChannels() const noexcept
{
    return m_nChannels;
}
