#include "textureTypes.h"

#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"

using namespace openglCore::texture;

TextureData::TextureData(unsigned char* textureData, GLsizei width, GLsizei height, unsigned int nChannels, 
    TexturePixelFormat format) : 
    m_data{ textureData }, m_height{ height }, m_width{ width }, m_nChannels{ nChannels }, 
    m_format{ format }
{
    OGLS_ASSERT(m_data);
    OGLS_ASSERT(m_height > 0);
    OGLS_ASSERT(m_width > 0);
    OGLS_ASSERT(m_nChannels > 0);
}

TextureData::TextureData(unsigned char* textureData, GLsizei width, GLsizei height, GLsizei depth,
    unsigned int nChannels, GLint level, TexturePixelFormat format, TextureInternalFormat internalFormat,
    TexturePixelType pixelType) :
    m_data{ textureData }, m_depth{ depth }, m_height{ height }, m_width{ width }, m_nChannels{ nChannels },
    m_level{ level }, m_format{ format }, m_internalFormat{ internalFormat }, m_type{ pixelType }
{
    OGLS_ASSERT(m_data);
    OGLS_ASSERT(m_height > 0);
    OGLS_ASSERT(m_width > 0);
    OGLS_ASSERT(m_nChannels > 0);
}

TextureData::~TextureData()
{
    helpers::freeTextureData(*this);
}
