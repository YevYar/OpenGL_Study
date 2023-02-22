#ifndef TEXTURE_TYPES_H
#define TEXTURE_TYPES_H

#include <glad/glad.h>

#include "helpers/macros.h"

namespace texture
{
    /**
     * \brief TextureData contains a pointer to the data of the texture, information about the loaded image 
     . and about some of the texture parameters.
     */
    class TextureData
    {
        public:
            TextureData() = delete;

            /**
             * \brief Constructs new object.
             *
             * TextureData takes an ownership of the data.
             * 
             * \param textureData - pointer to the data.
             * \param width - width of the texture in pixels.
             * \param height - height of the texture in pixels.
             * \param nChannels - a number of color channels of the image.
             * \param format - the format of image data.
             */
            TextureData(unsigned char* textureData, GLsizei width, GLsizei height, unsigned int nChannels,
                TexturePixelFormat format);

            /**
             * \brief Constructs new object.
             *
             * TextureData takes an ownership of the data.
             *
             * \param textureData - pointer to the data.
             * \param width - width of the texture in pixels.
             * \param height - height of the 2D or 3D texture in pixels. For 1D texture is not used.
             * \param depth - depth of the 3D texture in pixels. For 1D and 2D texture is not used.
             * \param nChannels - a number of color channels of the image.
             * \param level - the level-of-detail number.
             * \param format - the format of pixel data.
             * \param internalFormat - specify how the texture shall be stored in the GPU.
             */
            TextureData(unsigned char* textureData, GLsizei width, GLsizei height, GLsizei depth,
                unsigned int nChannels, GLint level, TexturePixelFormat format, TextureInternalFormat internalFormat);

            NOT_COPYABLE_MOVABLE(TextureData)

            /**
             * \brief Deletes the object and cleans the data by calling helpers::freeTextureData().
             */
            ~TextureData();

        public:
            /**
             * \brief The data of the texture.
             * 
             * DON'T delete, TextureData takes an ownership of the data.
             */
            unsigned char* m_data = nullptr;
            /**
             * \brief The depth in pixels of the 3D texture.
             */
            GLsizei m_depth = 0;
            /**
             * \brief The height in pixels of the 2D or 3D texture.
             */
            GLsizei m_height = 0;
            /**
             * \brief The width in pixels of the texture.
             */
            GLsizei m_width = 0;
            /**
             * \brief The level-of-detail number.
             */
            GLint m_level = 0;
            /**
             * \brief A number of color channels of the image.
             */
            unsigned int m_nChannels = 0;
            /**
             * \brief The format of pixel data.
             */
            TexturePixelFormat m_format = TexturePixelFormat::RGB;
            /**
             * \brief The internal format, in which the texture shall be stored in the GPU.
             */
            TextureInternalFormat m_internalFormat = TextureInternalFormat::RGB;

    };

    /**
     * \brief TextureTarget is a type of the texture, which represents 'target' parameter of
     * [glBindTexture()](https://docs.gl/gl4/glBindTexture).
     */
    enum class TextureBindingTarget : GLenum
    {
        TEXTURE_1D = 0x0DE0, TEXTURE_2D = 0x0DE1, TEXTURE_3D = 0x806F, TEXTURE_1D_ARRAY = 0x8C18,
        TEXTURE_2D_ARRAY = 0x8C1A, TEXTURE_RECTANGLE = 0x84F5, TEXTURE_CUBE_MAP = 0x8513,
        TEXTURE_CUBE_MAP_ARRAY = 0x9009, TEXTURE_BUFFER = 0x8C2A, TEXTURE_2D_MULTISAMPLE = 0x9100,
        TEXTURE_2D_MULTISAMPLE_ARRAY = 0x9102
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
        RED = 0x1903, RG = 0x8227, RGB = 0x1907, BGR = 0x80E0, RGBA = 0x1908, BGRA = 0x80E1, RED_INTEGER = 0x8D94,
        RG_INTEGER = 0x8228, RGB_INTEGER = 0x8D98, BGR_INTEGER = 0x8D9A, RGBA_INTEGER = 0x8D99, BGRA_INTEGER = 0x8D9B,
        STENCIL_INDEX = 0x1901, DEPTH_COMPONENT = 0x1902, DEPTH_STENCIL = 0x84F9
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
        DEPTH_COMPONENT = 0x1902, DEPTH_STENCIL = 0x84F9, RED = 0x1903, RG = 0x8227, RGB = 0x1907, RGBA = 0x1908,
        R8 = 0x8229, R8_SNORM = 0x8F94, R16 = 0x822A, R16_SNORM = 0x8F98, RG8 = 0x822B, RG8_SNORM = 0x8F95,
        RG16 = 0x822C, RG16_SNORM = 0x8F99, R3_G3_B2 = 0x2A10, RGB4 = 0x804F, RGB5 = 0x8050, RGB8 = 0x8051,
        RGB8_SNORM = 0x8F96, RGB10 = 0x8052, RGB12 = 0x8053, RGB16_SNORM = 0x8F9A, RGBA2 = 0x8055, RGBA4 = 0x8056,
        RGB5_A1 = 0x8057, RGBA8 = 0x8058, RGBA8_SNORM = 0x8F97, RGB10_A2 = 0x8059, RGB10_A2UI = 0x906F,
        RGBA12 = 0x805A, RGBA16 = 0x805B, SRGB8 = 0x8C41, SRGB8_ALPHA8 = 0x8C43, R16F = 0x822D, RG16F = 0x822F,
        RGB16F = 0x881B, RGBA16F = 0x881A, R32F = 0x822E, RG32F = 0x8230, RGB32F = 0x8815, RGBA32F = 0x8814,
        R11F_G11F_B10F = 0x8C3A, RGB9_E5 = 0x8C3D, R8I = 0x8231, R8UI = 0x8232, R16I = 0x8233, R16UI = 0x8234,
        R32I = 0x8235, R32UI = 0x8236, RG8I = 0x8237, RG8UI = 0x8238, RG16I = 0x8239, RG16UI = 0x823A,
        RG32I = 0x823B, RG32UI = 0x823C, RGB8I = 0x8D8F, RGB8UI = 0x8D7D, RGB16I = 0x8D89, RGB16UI = 0x8D77,
        RGB32I = 0x8D83, RGB32UI = 0x8D71, RGBA8I = 0x8D8E, RGBA8UI = 0x8D7C, RGBA16I = 0x8D88, RGBA16UI = 0x8D76,
        RGBA32I = 0x8D82, RGBA32UI = 0x8D70, COMPRESSED_RED = 0x8225, COMPRESSED_RG = 0x8226, COMPRESSED_RGB = 0x84ED,
        COMPRESSED_RGBA = 0x84EE, COMPRESSED_SRGB = 0x8C48, COMPRESSED_SRGB_ALPHA = 0x8C49,
        COMPRESSED_RED_RGTC1 = 0x8DBB, COMPRESSED_SIGNED_RED_RGTC1 = 0x8DBC, COMPRESSED_RG_RGTC2 = 0x8DBD,
        COMPRESSED_SIGNED_RG_RGTC2 = 0x8DBE, COMPRESSED_RGBA_BPTC_UNORM = 0x8E8C,
        COMPRESSED_SRGB_ALPHA_BPTC_UNORM = 0x8E8D, COMPRESSED_RGB_BPTC_SIGNED_FLOAT = 0x8E8E,
        COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT = 0x8E8F
    };

    /**
     * \brief TexturePixelType is a type of the pixel data, which represents 'type' parameter of such functions as
     * [glTexImage1D()](https://docs.gl/gl4/glTexImage1D), [glTexImage2D()](https://docs.gl/gl4/glTexImage2D),
     * [glTexImage3D()](https://docs.gl/gl4/glTexImage3D) etc.
     */
    enum class TexturePixelType : GLenum
    {
        UNSIGNED_BYTE = 0x1401, BYTE = 0x1400, UNSIGNED_SHORT = 0x1403, SHORT = 0x1402,
        UNSIGNED_INT = 0x1405, INT = 0x1404, FLOAT = 0x1406, UNSIGNED_BYTE_3_3_2 = 0x8032,
        UNSIGNED_BYTE_2_3_3_REV = 0x8362, UNSIGNED_SHORT_5_6_5 = 0x8363, UNSIGNED_SHORT_5_6_5_REV = 0x8364,
        UNSIGNED_SHORT_4_4_4_4 = 0x8033, UNSIGNED_SHORT_4_4_4_4_REV = 0x8365, UNSIGNED_SHORT_5_5_5_1 = 0x8034,
        UNSIGNED_SHORT_1_5_5_5_REV = 0x8366, UNSIGNED_INT_8_8_8_8 = 0x8035, UNSIGNED_INT_8_8_8_8_REV = 0x8367,
        UNSIGNED_INT_10_10_10_2 = 0x8036, UNSIGNED_INT_2_10_10_10_REV = 0x8368
    };

    /**
     * \brief TexImage1DTarget is a type of the texture, which represents 'target' parameter of
     * [glTexImage1D()](https://docs.gl/gl4/glTexImage1D).
     */
    enum class TexImage1DTarget : GLenum
    {
        TEXTURE_1D = 0x0DE0, PROXY_TEXTURE_1D = 0x8063
    };

    /**
     * \brief TexImage2DTarget is a type of the texture, which represents 'target' parameter of
     * [glTexImage2D()](https://docs.gl/gl4/glTexImage2D).
     */
    enum class TexImage2DTarget : GLenum
    {
        TEXTURE_2D = 0x0DE1, PROXY_TEXTURE_2D = 0x8064, TEXTURE_1D_ARRAY = 0x8C18, PROXY_TEXTURE_1D_ARRAY = 0x8C19,
        TEXTURE_RECTANGLE = 0x84F5, PROXY_TEXTURE_RECTANGLE = 0x84F7, TEXTURE_CUBE_MAP_POSITIVE_X = 0x8515,
        TEXTURE_CUBE_MAP_NEGATIVE_X = 0x8516, TEXTURE_CUBE_MAP_POSITIVE_Y = 0x8517,
        TEXTURE_CUBE_MAP_NEGATIVE_Y = 0x8518, TEXTURE_CUBE_MAP_POSITIVE_Z = 0x8519,
        TEXTURE_CUBE_MAP_NEGATIVE_Z = 0x851A, PROXY_TEXTURE_CUBE_MAP = 0x851B
    };

    /**
     * \brief TexImage3DTarget is a type of the texture, which represents 'target' parameter of
     * [glTexImage3D()](https://docs.gl/gl4/glTexImage3D).
     */
    enum class TexImage3DTarget : GLenum
    {
        TEXTURE_3D = 0x806F, PROXY_TEXTURE_3D = 0x8070, TEXTURE_2D_ARRAY = 0x8C1A, PROXY_TEXTURE_2D_ARRAY = 0x8C1B
    };

    /**
     * \brief TexParameterName represents 'pname' parameter of [glTexParameter()](https://docs.gl/gl4/glTexParameter).
     */
    enum class TexParameterName : GLenum
    {
        DEPTH_STENCIL_TEXTURE_MODE = 0x90EA, TEXTURE_BASE_LEVEL = 0x813C, TEXTURE_COMPARE_FUNC = 0x884D,
        TEXTURE_COMPARE_MODE = 0x884C, TEXTURE_LOD_BIAS = 0x8501, TEXTURE_MIN_FILTER = 0x2801,
        TEXTURE_MAG_FILTER = 0x2800, TEXTURE_MIN_LOD = 0x813A, TEXTURE_MAX_LOD = 0x813B, TEXTURE_MAX_LEVEL = 0x813D, 
        TEXTURE_SWIZZLE_R = 0x8E42, TEXTURE_SWIZZLE_G = 0x8E43, TEXTURE_SWIZZLE_B = 0x8E44, TEXTURE_SWIZZLE_A = 0x8E45,
        TEXTURE_WRAP_S = 0x2802, TEXTURE_WRAP_T = 0x2803, TEXTURE_WRAP_R = 0x8072
    };

    /**
     * \brief TexParameterVectorName represents 'pname' parameter of 
     * [glTexParameter*v()](https://docs.gl/gl4/glTexParameter).
     */
    enum class TexParameterVectorName : GLenum
    {
        TEXTURE_BORDER_COLOR = 0x1004, TEXTURE_SWIZZLE_RGBA = 0x8E46
    };
}

#endif
