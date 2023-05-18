#ifndef OGLS_OGLCORE_TEXTURE_TEXTURE_IMPL_H
#define OGLS_OGLCORE_TEXTURE_TEXTURE_IMPL_H

#include "texture.h"

#include "openglHelpersImpl.h"

namespace ogls::oglCore::texture
{
struct BaseTexture::BaseImpl
{
    public:
        BaseImpl() = default;
        explicit BaseImpl(TextureTarget target);
        BaseImpl(const BaseImpl& obj);
        virtual ~BaseImpl();

        BaseImpl& operator=(const BaseImpl&)     = delete;
        BaseImpl& operator=(BaseImpl&&) noexcept = delete;

        void deleteTexture() noexcept;
        void genTexture();

    public:
        /**
         * \brief Id of referenced OpenGL texture.
         */
        GLuint        rendererId = {0};
        TextureTarget target     = TextureTarget::Texture2d;

};  // struct BaseTexture::BaseImpl

template<unsigned int DimensionsNumber>
struct TexDimensionSpecificFunc
{
};

template<>
struct TexDimensionSpecificFunc<1>
{
    public:
        void setTexImageInTarget(GLuint textureId, std::shared_ptr<TextureData> textureData);
        void setTexStorageFormat(GLuint textureId, const std::shared_ptr<TextureData>& textureData) const noexcept;

};  // struct TexDimensionSpecificFunc<1>

template<>
struct TexDimensionSpecificFunc<2>
{
    public:
        void setTexImageInTarget(GLuint textureId, std::shared_ptr<TextureData> textureData);
        void setTexStorageFormat(GLuint textureId, const std::shared_ptr<TextureData>& textureData) const noexcept;

};  // struct TexDimensionSpecificFunc<2>

template<>
struct TexDimensionSpecificFunc<3>
{
    public:
        void setTexImageInTarget(GLuint textureId, std::shared_ptr<TextureData> textureData);
        void setTexStorageFormat(GLuint textureId, const std::shared_ptr<TextureData>& textureData) const noexcept;

};  // struct TexDimensionSpecificFunc<3>

template<unsigned int DimensionsNumber>
struct Texture<DimensionsNumber>::Impl : public BaseTexture::BaseImpl
{
    public:
        static void                 bindToTarget(TextureTarget target, GLuint textureId);
        static TextureBindingTarget getTargetAssociatedGetParameter(TextureTarget target) noexcept;

        explicit Impl(TextureTarget target);
        Impl(const Impl& obj);

        Impl& operator=(const Impl&)     = delete;
        Impl& operator=(Impl&&) noexcept = delete;

        void bind() const noexcept;
        void setData(std::shared_ptr<TextureData> textureData);
        void specifyTextureStorageFormat(const std::shared_ptr<TextureData>& textureData) const noexcept;

    public:
        std::shared_ptr<TextureData>               data;
        const GLuint                               dimensionsNumber         = {DimensionsNumber};
        mutable bool                               isStorageFormatSpecified = false;
        TexDimensionSpecificFunc<DimensionsNumber> specific;


        template<OpenGLBindableObject Type>
        friend void ogls::oglCore::bindForAMomentAndExecute(const Type&, const std::function<void()>&);

};  // struct Texture<DimensionsNumber>::Impl

}  // namespace ogls::oglCore::texture

#endif
