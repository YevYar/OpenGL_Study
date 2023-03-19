#include "textureTypes.h"

#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"

using namespace openglCore::texture;

TextureData::TextureData(unsigned char* textureData, GLsizei width, GLsizei height, unsigned int nChannels, 
    TexturePixelFormat format) : 
    m_data{ textureData }, m_height{ height }, m_width{ width }, m_nChannels{ nChannels }, 
    m_format{ format }
{
    ASSERT(m_data);
    ASSERT(m_height > 0);
    ASSERT(m_width > 0);
    ASSERT(m_nChannels > 0);
}

TextureData::TextureData(unsigned char* textureData, GLsizei width, GLsizei height, GLsizei depth,
    unsigned int nChannels, GLint level, TexturePixelFormat format, TextureInternalFormat internalFormat,
    TexturePixelType pixelType) :
    m_data{ textureData }, m_depth{ depth }, m_height{ height }, m_width{ width }, m_nChannels{ nChannels },
    m_level{ level }, m_format{ format }, m_internalFormat{ internalFormat }, m_type{ pixelType }
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
