#ifndef TEXTURE_IMPL_H
#define TEXTURE_IMPL_H

#include "texture.h"

namespace texture
{
    struct BaseTexture::Impl
    {
        Impl(TextureTarget target);
        Impl(const Impl& obj);
        Impl(Impl&& obj) noexcept;

        Impl& operator=(const Impl&) = delete;
        Impl& operator=(Impl&&) noexcept = delete;

        /**
         * \brief Id of referenced OpenGL texture.
         */
        GLuint m_rendererId = 0;
        TextureTarget m_target = TextureTarget::TEXTURE_2D;

    };

    template<unsigned int DimensionsNumber>
    struct TexDimensionSpecificTypesAndFunc
    {
    };

    template<unsigned int DimensionsNumber>
    struct Texture<DimensionsNumber>::Impl : public TexDimensionSpecificTypesAndFunc<DimensionsNumber>
    {
        // TexDimensionSpecificTypesAndFunc<DimensionsNumber>* m_dimensionTypesAndFunc;

        static void bindToTarget(TextureTarget target, GLuint textureId) noexcept;
        static TextureBindingTarget getTargetAssociatedGetParameter(TextureTarget target) noexcept;

        void genTexture();
        void deleteTexture() noexcept;

        const GLuint m_dimensionsNumber = DimensionsNumber;
        TexImageTarget texImageTarget;
        TexImageTarget m_lastTexImageTarget;
        std::shared_ptr<TextureData> m_data;
        mutable bool m_isStorageFormatSpecified = false;

    };
}

#endif
