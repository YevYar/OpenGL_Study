#ifndef TEXTURE_IMPL_H
#define TEXTURE_IMPL_H

#include "texture.h"

namespace texture
{
    struct BaseTexture::BaseImpl
    {
        BaseImpl() = default;
        BaseImpl(TextureTarget target);
        BaseImpl(const BaseImpl& obj);

        virtual ~BaseImpl();

        BaseImpl& operator=(const BaseImpl&) = delete;
        BaseImpl& operator=(BaseImpl&&) noexcept = delete;

        void genTexture();
        void deleteTexture() noexcept;

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
    struct Texture<DimensionsNumber>::Impl : public BaseTexture::BaseImpl,
        public TexDimensionSpecificTypesAndFunc<DimensionsNumber>
    {
        using TexImageTarget = TexDimensionSpecificTypesAndFunc<DimensionsNumber>::TexImageTarget;
        // TexDimensionSpecificTypesAndFunc<DimensionsNumber>* m_dimensionTypesAndFunc;
        Impl(TextureTarget target);
        Impl(const Impl& obj);

        Impl& operator=(const Impl&) = delete;
        Impl& operator=(Impl&&) noexcept = delete;

        static void bindToTarget(TextureTarget target, GLuint textureId) noexcept;
        static TextureBindingTarget getTargetAssociatedGetParameter(TextureTarget target) noexcept;

        void specifyTextureStorageFormat(const std::shared_ptr<TextureData>& textureData) const noexcept;
        void setData(TexImageTarget texImageTarget, std::shared_ptr<TextureData> textureData);

        const GLuint m_dimensionsNumber = DimensionsNumber;
        TexImageTarget m_texImageTarget;
        TexImageTarget m_lastTexImageTarget;
        std::shared_ptr<TextureData> m_data;
        mutable bool m_isStorageFormatSpecified = false;

    };
}

#endif
