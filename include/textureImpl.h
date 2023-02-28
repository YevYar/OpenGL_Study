#ifndef TEXTURE_IMPL_H
#define TEXTURE_IMPL_H

#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"
#include "textureTypes.h"

namespace texture
{
    template<unsigned int DimensionsNumber>
    struct TexDimensionSpecificTypesAndFunc
    {
    };

    template<>
    struct TexDimensionSpecificTypesAndFunc<1>
    {
        TexImage1DTarget texImageTarget;

        void setTexStorageFormat(GLuint textureId, const std::shared_ptr<TextureData>& textureData) const noexcept
        {
            GLCall(glTextureStorage1D(textureId, textureData->m_level, helpers::toUType(textureData->m_internalFormat),
                textureData->m_width));
        }

        void setTexImageInTarget(GLuint textureId, TexImage1DTarget target, std::shared_ptr<TextureData> textureData)
        {
            using namespace helpers;

            GLCall(glTextureSubImage1D(textureId, 0, 0, textureData->m_width, toUType(textureData->m_format),
                toUType(textureData->m_type), textureData->m_data));
        }
    };

    template<>
    struct TexDimensionSpecificTypesAndFunc<2>
    {
        TexImage2DTarget texImageTarget;

        void setTexStorageFormat(GLuint textureId, const std::shared_ptr<TextureData>& textureData) const noexcept
        {
            GLCall(glTextureStorage2D(textureId, textureData->m_level, helpers::toUType(textureData->m_internalFormat),
                textureData->m_width, textureData->m_height));
        }

        void setTexImageInTarget(GLuint textureId, TexImage2DTarget target, std::shared_ptr<TextureData> textureData)
        {
            using namespace helpers;            
            
            GLCall(glTextureSubImage2D(textureId, 0, 0, 0, textureData->m_width, textureData->m_height,
                toUType(textureData->m_format), toUType(textureData->m_type), textureData->m_data));
        }
    };

    template<>
    struct TexDimensionSpecificTypesAndFunc<3>
    {
        TexImage3DTarget texImageTarget;

        void setTexStorageFormat(GLuint textureId, const std::shared_ptr<TextureData>& textureData) const noexcept
        {
            GLCall(glTextureStorage3D(textureId, textureData->m_level, helpers::toUType(textureData->m_internalFormat),
                textureData->m_width, textureData->m_height, textureData->m_depth));
        }

        void setTexImageInTarget(GLuint textureId, TexImage3DTarget target, std::shared_ptr<TextureData> textureData)
        {
            using namespace helpers;            
            
            GLCall(glTextureSubImage3D(textureId, 0, 0, 0, 0, textureData->m_width, textureData->m_height,
                textureData->m_depth, toUType(textureData->m_format), toUType(textureData->m_type),
                textureData->m_data));
        }
    };
}

#endif
