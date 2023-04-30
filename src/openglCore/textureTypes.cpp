#include "textureTypes.h"

#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"

namespace ogls::oglCore::texture
{
TextureData::TextureData(unsigned char* textureData, GLsizei w, GLsizei h, unsigned int nCh, TexturePixelFormat f) :
    data{textureData}, height{h}, width{w}, nChannels{nCh}, format{f}
{
    OGLS_ASSERT(data);
    OGLS_ASSERT(height > 0);
    OGLS_ASSERT(width > 0);
    OGLS_ASSERT(nChannels > 0);
}

TextureData::TextureData(unsigned char* textureData, GLsizei w, GLsizei h, GLsizei d, unsigned int nCh, GLint l,
                         TexturePixelFormat f, TextureInternalFormat iF, TexturePixelType pixelType) :
    data{textureData},
    depth{d}, height{h}, width{w}, nChannels{nCh}, level{l}, format{f}, internalFormat{iF}, type{pixelType}
{
    OGLS_ASSERT(data);
    OGLS_ASSERT(height > 0);
    OGLS_ASSERT(width > 0);
    OGLS_ASSERT(nChannels > 0);
}

TextureData::~TextureData()
{
    helpers::freeTextureData(*this);
}

}  // namespace ogls::oglCore::texture
