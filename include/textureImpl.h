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

        void setTexImageInTarget(TexImage1DTarget target, std::shared_ptr<TextureData> textureData)
        {
            using namespace helpers;
            GLCall(glTexImage1D(toUType(target), textureData->m_level, toUType(textureData->m_internalFormat),
                textureData->m_width, 0, toUType(textureData->m_format), toUType(textureData->m_type), textureData->m_data));
        }
    };

    template<>
    struct TexDimensionSpecificTypesAndFunc<2>
    {
        TexImage2DTarget texImageTarget;

        void setTexImageInTarget(TexImage2DTarget target, std::shared_ptr<TextureData> textureData)
        {
            using namespace helpers;
            GLCall(glTexImage2D(toUType(target), textureData->m_level, toUType(textureData->m_internalFormat), textureData->m_width,
                textureData->m_height, 0, toUType(textureData->m_format), toUType(textureData->m_type), textureData->m_data));
        }
    };

    template<>
    struct TexDimensionSpecificTypesAndFunc<3>
    {
        TexImage3DTarget texImageTarget;

        void setTexImageInTarget(TexImage3DTarget target, std::shared_ptr<TextureData> textureData)
        {
            using namespace helpers;
            GLCall(glTexImage3D(toUType(target), textureData->m_level, toUType(textureData->m_internalFormat), textureData->m_width,
                textureData->m_height, textureData->m_depth, 0, toUType(textureData->m_format), toUType(textureData->m_type),
                textureData->m_data));
        }
    };
}

#endif
