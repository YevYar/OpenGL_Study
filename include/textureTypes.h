#ifndef TEXTURE_TYPES_H
#define TEXTURE_TYPES_H

#include "helpers/macros.h"

namespace texture
{
    /**
     * \brief TextureData contains a pointer to the data of the texture and information about the loaded image.
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
             * \param width - width of the texture in pixels.
             * \param height - height of the texture in pixels.
             * \param nChannels - a number of color channels of the image.
             */
            TextureData(unsigned char* textureData, int width, int height, int nChannels);

            NOT_COPYABLE_MOVABLE(TextureData)

            /**
             * \brief Deletes the object and cleans the data by calling helpers::freeTextureData().
             */
            ~TextureData();

            /**
             * \brief Returns the data of the texture.
             * 
             * \return pointer to the data.
             */
            unsigned char* getData() const noexcept;

            /**
             * \brief Returns the height of the texture.
             *
             * \return height of the texture in pixels.
             */
            int getHeight() const noexcept;

            /**
             * \brief Returns the width of the texture.
             *
             * \return width of the texture in pixels.
             */
            int getWidth() const noexcept;

            /**
             * \brief Returns the number of color channels in the image of the texture.
             *
             * \return number of color channels in the image of the texture.
             */
            int getNChannels() const noexcept;

        private:
            unsigned char* m_data = nullptr;
            int m_height = 0;
            int m_width = 0;
            int m_nChannels = 0;

    };

    /**
     * \brief TextureTarget is a type of the texture, which represents 'target' parameter of
     * [glBindTexture()](https://docs.gl/gl4/glBindTexture).
     */
    enum class TextureBindingTarget : unsigned int
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
     */
    enum class TexturePixelFormat : unsigned int
    {
        RED = 0x1903, RG = 0x8227, RGB = 0x1907, BGR = 0x80E0, RGBA = 0x1908, BGRA = 0x80E1, RED_INTEGER = 0x8D94,
        RG_INTEGER = 0x8228, RGB_INTEGER = 0x8D98, BGR_INTEGER = 0x8D9A, RGBA_INTEGER = 0x8D99, BGRA_INTEGER = 0x8D9B,
        STENCIL_INDEX = 0x1901, DEPTH_COMPONENT = 0x1902, DEPTH_STENCIL = 0x84F9
    };

    /**
     * \brief TexturePixelType is a type of the pixel data, which represents 'type' parameter of such functions as
     * [glTexImage1D()](https://docs.gl/gl4/glTexImage1D), [glTexImage2D()](https://docs.gl/gl4/glTexImage2D),
     * [glTexImage3D()](https://docs.gl/gl4/glTexImage3D) etc.
     */
    enum class TexturePixelType : unsigned int
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
    enum class TexImage1DTarget : unsigned int
    {
        TEXTURE_1D = 0x0DE0, PROXY_TEXTURE_1D = 0x8063
    };

    /**
     * \brief TexImage2DTarget is a type of the texture, which represents 'target' parameter of
     * [glTexImage2D()](https://docs.gl/gl4/glTexImage2D).
     */
    enum class TexImage2DTarget : unsigned int
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
    enum class TexImage3DTarget : unsigned int
    {
        TEXTURE_3D = 0x806F, PROXY_TEXTURE_3D = 0x8070, TEXTURE_2D_ARRAY = 0x8C1A, PROXY_TEXTURE_2D_ARRAY = 0x8C1B
    };

    /**
     * \brief TexParameterName represents 'pname' parameter of [glTexParameter()](https://docs.gl/gl4/glTexParameter).
     */
    enum class TexParameterName : unsigned int
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
    enum class TexParameterVectorName : unsigned int
    {
        TEXTURE_BORDER_COLOR = 0x1004, TEXTURE_SWIZZLE_RGBA = 0x8E46
    };
}

#endif
