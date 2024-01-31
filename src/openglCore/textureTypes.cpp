#include "textureTypes.h"

#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"

namespace ogls::oglCore::texture
{
TextureData::TextureData(DataType textureData, GLsizei w, GLsizei h, int nCh, TexturePixelFormat f) noexcept :
    data{std::move(textureData)}, height{h > 0 ? h : 0}, width{w > 0 ? w : 0}, nChannels{nCh > 0 ? nCh : 0}, format{f}
{
    OGLS_ASSERT(data);
    OGLS_ASSERT(h > 0);
    OGLS_ASSERT(w > 0);
    OGLS_ASSERT(nCh > 0);
}

TextureData::TextureData(DataType textureData, GLsizei w, GLsizei h, GLsizei d, int nCh, GLint l, TexturePixelFormat f,
                         TextureInternalFormat iF, TexturePixelType pixelType) noexcept :
    data{std::move(textureData)},
    depth{d > 0 ? d : 0}, height{h > 0 ? h : 0}, width{w > 0 ? w : 0}, nChannels{nCh > 0 ? nCh : 0},
    level{l > 0 ? l : 0}, format{f}, internalFormat{iF}, type{pixelType}
{
    OGLS_ASSERT(data);
    OGLS_ASSERT(d > 0);
    OGLS_ASSERT(h > 0);
    OGLS_ASSERT(w > 0);
    OGLS_ASSERT(nCh > 0);
    OGLS_ASSERT(l > 0);
}

}  // namespace ogls::oglCore::texture
