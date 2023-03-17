#ifndef TEXTURE_IMPL_H
#define TEXTURE_IMPL_H

#include <glad/glad.h>

#include "helpers/helpers.h"
#include "openglHelpers.h"
#include "texture.h"

namespace texture
{
    struct BaseTexture::BaseImpl
    {
        public:
            BaseImpl() = default;
            explicit BaseImpl(TextureTarget target);
            BaseImpl(const BaseImpl& obj);

            virtual ~BaseImpl();

            BaseImpl& operator=(const BaseImpl&) = delete;
            BaseImpl& operator=(BaseImpl&&) noexcept = delete;

            void genTexture();
            void deleteTexture() noexcept;

        public:
            /**
             * \brief Id of referenced OpenGL texture.
             */
            GLuint m_rendererId = 0;
            TextureTarget m_target = TextureTarget::TEXTURE_2D;

    };

    template<unsigned int DimensionsNumber>
    struct TexDimensionSpecificFunc
    {
    };

    template<>
    struct TexDimensionSpecificFunc<1>
    {
        using TexImageTarget = TexDimensionSpecificTypes<1>::TexImageTarget;
        void setTexStorageFormat(GLuint textureId, const std::shared_ptr<TextureData>& textureData) const noexcept;
        void setTexImageInTarget(GLuint textureId, TexImageTarget target, std::shared_ptr<TextureData> textureData);
    };

    template<>
    struct TexDimensionSpecificFunc<2>
    {
        using TexImageTarget = TexDimensionSpecificTypes<2>::TexImageTarget;
        void setTexStorageFormat(GLuint textureId, const std::shared_ptr<TextureData>& textureData) const noexcept;
        void setTexImageInTarget(GLuint textureId, TexImageTarget target, std::shared_ptr<TextureData> textureData);
    };

    template<>
    struct TexDimensionSpecificFunc<3>
    {
        using TexImageTarget = TexDimensionSpecificTypes<3>::TexImageTarget;
        void setTexStorageFormat(GLuint textureId, const std::shared_ptr<TextureData>& textureData) const noexcept;
        void setTexImageInTarget(GLuint textureId, TexImageTarget target, std::shared_ptr<TextureData> textureData);
    };

    template<unsigned int DimensionsNumber>
    struct Texture<DimensionsNumber>::Impl : public BaseTexture::BaseImpl
    {
        public:
            using TexImageTarget = TexDimensionSpecificFunc<DimensionsNumber>::TexImageTarget;
        

            explicit Impl(TextureTarget target);
            Impl(const Impl& obj);

            Impl& operator=(const Impl&) = delete;
            Impl& operator=(Impl&&) noexcept = delete;

            static void bindToTarget(TextureTarget target, GLuint textureId) noexcept;
            static TextureBindingTarget getTargetAssociatedGetParameter(TextureTarget target) noexcept;

            void bind() const noexcept;
            void specifyTextureStorageFormat(const std::shared_ptr<TextureData>& textureData) const noexcept;
            void setData(TexImageTarget texImageTarget, std::shared_ptr<TextureData> textureData);

        public:
            TexDimensionSpecificFunc<DimensionsNumber> m_specific;
            const GLuint m_dimensionsNumber = DimensionsNumber;
            TexImageTarget m_texImageTarget;
            TexImageTarget m_lastTexImageTarget;
            std::shared_ptr<TextureData> m_data;
            mutable bool m_isStorageFormatSpecified = false;

        template<openglCore::OpenGLBindableObject Type>
        friend void openglCore::bindForAMomentAndExecute(const Type&, const std::function<void()>&);

    };
}

#endif
