#ifndef OGLS_OGLCORE_TEXTURE_TEXTURE_TYPES_H
#define OGLS_OGLCORE_TEXTURE_TEXTURE_TYPES_H

#include <glad/glad.h>

#include "helpers/macros.h"

namespace ogls::oglCore::texture
{
/**
 * \brief TexParameterName represents 'pname' parameter of [glTexParameter()](https://docs.gl/gl4/glTexParameter).
 */
enum class TexParameterName : GLenum
{
    DepthStencilTextureMode = 0x90'EA,
    TextureBaseLevel        = 0x81'3C,
    TextureCompareFunc      = 0x88'4D,
    TextureCompareMode      = 0x88'4C,
    TextureLodBias          = 0x85'01,
    TextureMagFilter        = 0x28'00,
    TextureMaxLevel         = 0x81'3D,
    TextureMaxLod           = 0x81'3B,
    TextureMinFilter        = 0x28'01,
    TextureMinLod           = 0x81'3A,
    TextureSwizzleA         = 0x8E'45,
    TextureSwizzleB         = 0x8E'44,
    TextureSwizzleG         = 0x8E'43,
    TextureSwizzleR         = 0x8E'42,
    TextureWrapR            = 0x80'72,
    TextureWrapS            = 0x28'02,
    TextureWrapT            = 0x28'03
};

/**
 * \brief TexParameterVectorName represents 'pname' parameter of
 * [glTexParameter*v()](https://docs.gl/gl4/glTexParameter).
 */
enum class TexParameterVectorName : GLenum
{
    TextureBorderColor = 0x10'04,
    TextureSwizzleRgba = 0x8E'46
};

/**
 * \brief TextureBindingTarget is a parameter by which the current bound texture to specific texture target
 * can be got. See 'Associated Gets' section of [glBindTexture()](https://docs.gl/gl4/glBindTexture).
 */
enum class TextureBindingTarget : GLenum
{
    TextureBinding1d                 = 0x80'68,
    TextureBinding1dArray            = 0x8C'1C,
    TextureBinding2d                 = 0x80'69,
    TextureBinding2dArray            = 0x8C'1D,
    TextureBinding2dMultisample      = 0x91'04,
    TextureBinding2dMultisampleArray = 0x91'05,
    TextureBinding3d                 = 0x80'6A,
    TextureBindingBuffer             = 0x8C'2C,
    TextureBindingCubeMap            = 0x85'14,
    TextureBindingCubeMapArray       = 0x90'0A,
    TextureBindingRectangle          = 0x84'F6
};

/**
 * \brief TextureInternalFormat is a format of the texture, which represents 'internalFormat' parameter
 * of such functions as [glTexImage1D()](https://docs.gl/gl4/glTexImage1D),
 * [glTexImage2D()](https://docs.gl/gl4/glTexImage2D), [glTexImage3D()](https://docs.gl/gl4/glTexImage3D) etc.
 *
 * The 'internalFormat' describes how the texture shall be stored in the GPU.
 * See this [explanation](https://stackoverflow.com/questions/34497195/difference-between-format-and-internalformat).
 */
enum class TextureInternalFormat : GLint
{
    CompressedRed                  = 0x82'25,
    CompressedRedRgtc1             = 0x8D'BB,
    CompressedRg                   = 0x82'26,
    CompressedRgRgtc2              = 0x8D'BD,
    CompressedRgb                  = 0x84'ED,
    CompressedRgbBptcSignedFloat   = 0x8E'8E,
    CompressedRgbBptcUnsignedFloat = 0x8E'8F,
    CompressedRgba                 = 0x84'EE,
    CompressedRgbaBptcUnorm        = 0x8E'8C,
    CompressedSignedRedRgtc1       = 0x8D'BC,
    CompressedSignedRgRgtc2        = 0x8D'BE,
    CompressedSrgb                 = 0x8C'48,
    CompressedSrgbAlpha            = 0x8C'49,
    CompressedSrgbAlphaBptcUnorm   = 0x8E'8D,
    DepthComponent                 = 0x19'02,
    DepthStencil                   = 0x84'F9,
    R11fG11fB10f                   = 0x8C'3A,
    R16                            = 0x82'2A,
    R16Snorm                       = 0x8F'98,
    R16f                           = 0x82'2D,
    R16i                           = 0x82'33,
    R16ui                          = 0x82'34,
    R32f                           = 0x82'2E,
    R32i                           = 0x82'35,
    R32ui                          = 0x82'36,
    R3G3B2                         = 0x2A'10,
    R8                             = 0x82'29,
    R8Snorm                        = 0x8F'94,
    R8i                            = 0x82'31,
    R8ui                           = 0x82'32,
    Red                            = 0x19'03,
    Rg                             = 0x82'27,
    Rg16                           = 0x82'2C,
    Rg16Snorm                      = 0x8F'99,
    Rg16f                          = 0x82'2F,
    Rg16i                          = 0x82'39,
    Rg16ui                         = 0x82'3A,
    Rg32f                          = 0x82'30,
    Rg32i                          = 0x82'3B,
    Rg32ui                         = 0x82'3C,
    Rg8                            = 0x82'2B,
    Rg8Snorm                       = 0x8F'95,
    Rg8i                           = 0x82'37,
    Rg8ui                          = 0x82'38,
    Rgb                            = 0x19'07,
    Rgb10                          = 0x80'52,
    Rgb10A2                        = 0x80'59,
    Rgb10A2ui                      = 0x90'6F,
    Rgb12                          = 0x80'53,
    Rgb16Snorm                     = 0x8F'9A,
    Rgb16f                         = 0x88'1B,
    Rgb16i                         = 0x8D'89,
    Rgb16ui                        = 0x8D'77,
    Rgb32f                         = 0x88'15,
    Rgb32i                         = 0x8D'83,
    Rgb32ui                        = 0x8D'71,
    Rgb4                           = 0x80'4F,
    Rgb5                           = 0x80'50,
    Rgb5A1                         = 0x80'57,
    Rgb8                           = 0x80'51,
    Rgb8Snorm                      = 0x8F'96,
    Rgb8i                          = 0x8D'8F,
    Rgb8ui                         = 0x8D'7D,
    Rgb9E5                         = 0x8C'3D,
    Rgba                           = 0x19'08,
    Rgba12                         = 0x80'5A,
    Rgba16                         = 0x80'5B,
    Rgba16f                        = 0x88'1A,
    Rgba16i                        = 0x8D'88,
    Rgba16ui                       = 0x8D'76,
    Rgba2                          = 0x80'55,
    Rgba32f                        = 0x88'14,
    Rgba32i                        = 0x8D'82,
    Rgba32ui                       = 0x8D'70,
    Rgba4                          = 0x80'56,
    Rgba8                          = 0x80'58,
    Rgba8Snorm                     = 0x8F'97,
    Rgba8i                         = 0x8D'8E,
    Rgba8ui                        = 0x8D'7C,
    Srgb8                          = 0x8C'41,
    Srgb8Alpha8                    = 0x8C'43
};

/**
 * \brief TexturePixelFormat is a format of the texture, which represents 'format' parameter of such functions as
 * [glTexImage1D()](https://docs.gl/gl4/glTexImage1D), [glTexImage2D()](https://docs.gl/gl4/glTexImage2D),
 * [glTexImage3D()](https://docs.gl/gl4/glTexImage3D) etc.
 *
 * The 'format' describes the format of pixel data in client memory.
 * See this [explanation](https://stackoverflow.com/questions/34497195/difference-between-format-and-internalformat).
 */
enum class TexturePixelFormat : GLenum
{
    Bgr            = 0x80'E0,
    BgrInteger     = 0x8D'9A,
    Bgra           = 0x80'E1,
    BgraInteger    = 0x8D'9B,
    DepthComponent = 0x19'02,
    DepthStencil   = 0x84'F9,
    Red            = 0x19'03,
    RedInteger     = 0x8D'94,
    Rg             = 0x82'27,
    RgInteger      = 0x82'28,
    Rgb            = 0x19'07,
    RgbInteger     = 0x8D'98,
    Rgba           = 0x19'08,
    RgbaInteger    = 0x8D'99,
    StencilIndex   = 0x19'01
};

/**
 * \brief TexturePixelType is a type of the pixel data, which represents 'type' parameter of such functions as
 * [glTexImage1D()](https://docs.gl/gl4/glTexImage1D), [glTexImage2D()](https://docs.gl/gl4/glTexImage2D),
 * [glTexImage3D()](https://docs.gl/gl4/glTexImage3D) etc.
 */
enum class TexturePixelType : GLenum
{
    Byte                  = 0x14'00,
    Float                 = 0x14'06,
    Int                   = 0x14'04,
    Short                 = 0x14'02,
    UnsignedByte          = 0x14'01,
    UnsignedByte233Rev    = 0x83'62,
    UnsignedByte332       = 0x80'32,
    UnsignedInt           = 0x14'05,
    UnsignedInt1010102    = 0x80'36,
    UnsignedInt2101010Rev = 0x83'68,
    UnsignedInt8888       = 0x80'35,
    UnsignedInt8888Rev    = 0x83'67,
    UnsignedShort         = 0x14'03,
    UnsignedShort1555Rev  = 0x83'66,
    UnsignedShort4444     = 0x80'33,
    UnsignedShort4444Rev  = 0x83'65,
    UnsignedShort5551     = 0x80'34,
    UnsignedShort565      = 0x83'63,
    UnsignedShort565Rev   = 0x83'64
};

/**
 * \brief TextureTarget is a type of the texture, which represents 'target' parameter of
 * [glBindTexture()](https://docs.gl/gl4/glBindTexture).
 */
enum class TextureTarget : GLenum
{
    Texture1d                 = 0x0D'E0,
    Texture1dArray            = 0x8C'18,
    Texture2d                 = 0x0D'E1,
    Texture2dArray            = 0x8C'1A,
    Texture2dMultisample      = 0x91'00,
    Texture2dMultisampleArray = 0x91'02,
    Texture3d                 = 0x80'6F,
    TextureBuffer             = 0x8C'2A,
    TextureCubeMap            = 0x85'13,
    TextureCubeMapArray       = 0x90'09,
    TextureRectangle          = 0x84'F5
};

/**
 * \brief TextureData contains a pointer to the data of the texture, information about the loaded image
 * and about some of the texture parameters.
 */
class TextureData
{
    public:
        /**
         * \brief Constructs new object.
         *
         * TextureData takes an ownership of the data.
         *
         * \param textureData - pointer to the data.
         * \param width       - width of the texture in pixels.
         * \param height      - height of the texture in pixels.
         * \param nChannels   - a number of color channels of the image.
         * \param format      - the format of image data.
         */
        TextureData(unsigned char* textureData, GLsizei width, GLsizei height, int nChannels,
                    TexturePixelFormat format) noexcept;
        /**
         * \brief Constructs new object.
         *
         * TextureData takes an ownership of the data.
         *
         * \param textureData    - pointer to the data.
         * \param width          - width of the texture in pixels.
         * \param height         - height of the 2D or 3D texture in pixels. For 1D texture is not used.
         * \param depth          - depth of the 3D texture in pixels. For 1D and 2D texture is not used.
         * \param nChannels      - a number of color channels of the image.
         * \param level          - the level-of-detail number.
         * \param format         - the format of pixel data.
         * \param internalFormat - specify how the texture shall be stored in the GPU.
         * \param pixelType      - the type of the pixel data.
         */
        TextureData(unsigned char* textureData, GLsizei width, GLsizei height, GLsizei depth, int nChannels,
                    GLint level, TexturePixelFormat format, TextureInternalFormat internalFormat,
                    TexturePixelType pixelType) noexcept;
        TextureData() = delete;
        OGLS_NOT_COPYABLE_MOVABLE(TextureData)
        /**
         * \brief Deletes the object and cleans the data by calling helpers::freeTextureData().
         */
        ~TextureData() noexcept;

    public:
        /**
         * \brief The data of the texture.
         *
         * DON'T delete, TextureData takes an ownership of the data.
         */
        unsigned char*        data           = nullptr;
        /**
         * \brief The depth in pixels of the 3D texture.
         */
        GLsizei               depth          = {0};
        /**
         * \brief The format of pixel data.
         */
        TexturePixelFormat    format         = TexturePixelFormat::Rgb;
        /**
         * \brief The height in pixels of the 2D or 3D texture.
         */
        GLsizei               height         = {0};
        /**
         * \brief The internal format, in which the texture shall be stored in the GPU.
         */
        TextureInternalFormat internalFormat = TextureInternalFormat::Rgb8;
        /**
         * \brief The level-of-detail number.
         */
        GLint                 level          = {1};
        /**
         * \brief A number of color channels of the image.
         */
        int                   nChannels      = {0};
        /**
         * \brief The type of the pixel data.
         */
        TexturePixelType      type           = TexturePixelType::UnsignedByte;
        /**
         * \brief The width in pixels of the texture.
         */
        GLsizei               width          = {0};

};  // class TextureData

}  // namespace ogls::oglCore::texture

#endif
