#ifndef OGLS_OGLCORE_TEXTURE_TEXTURE_IMPL_H
#define OGLS_OGLCORE_TEXTURE_TEXTURE_IMPL_H

#include "texture.h"

#include "openglHelpersImpl.h"

namespace ogls::oglCore::texture
{
/**
 * \brief BaseImpl contains private data and methods of BaseTexture.
 */
class BaseTexture::BaseImpl
{
    public:
        /**
         * \see genTexture().
         */
        explicit BaseImpl(TextureTarget target);
        /**
         * \see genTexture().
         */
        BaseImpl(const BaseImpl& obj);
        /**
         * \see deleteTexture().
         */
        virtual ~BaseImpl() noexcept;

        BaseImpl& operator=(const BaseImpl&)     = delete;
        BaseImpl& operator=(BaseImpl&&) noexcept = delete;

        /**
         * \brief Deletes the texture in OpenGL state machine.
         *
         * Wraps [glDeleteTextures()](https://docs.gl/gl4/glDeleteTextures).
         */
        void deleteTexture();
        /**
         * \brief Generates new 1 texture in OpenGL state machine.
         *
         * Wraps [glCreateTextures()](https://docs.gl/gl4/glCreateTextures).
         *
         * \throw ogls::exceptions::GLRecAcquisitionException().
         */
        void genTexture();

    public:
        /**
         * \brief ID of referenced OpenGL texture.
         */
        GLuint        rendererId = {0};
        /**
         * \brief Target to bind texture to (in other words, type of the texture).
         */
        TextureTarget target     = TextureTarget::Texture2d;

};  // class BaseTexture::BaseImpl

/**
 * \brief TexDimensionSpecificFunc declares some functions, which are used for the same purpose,
 * but by different texture types depending on their dimension (1D, 2D, 3D).
 *
 * \param DimensionsNumber - the integer value in the range [1, 3], which specifies a number of dimensions in the
 * texture.
 */
template<size_t DimensionsNumber>
class TexDimensionSpecificFunc
{
};

template<>
class TexDimensionSpecificFunc<1>
{
    public:
        /**
         * \brief Wraps [glTextureSubImage1D()](https://docs.gl/gl4/glTexSubImage1D).
         *
         * \param textureId - rendererId of referenced OpenGL texture.
         */
        void setTexImageInTarget(GLuint textureId, std::shared_ptr<TextureData> textureData);
        /**
         * \brief Wraps [glTextureStorage1D()](https://docs.gl/gl4/glTexStorage1D).
         *
         * \param textureId - rendererId of referenced OpenGL texture.
         */
        void setTexStorageFormat(GLuint textureId, const std::shared_ptr<TextureData>& textureData);

};  // class TexDimensionSpecificFunc<1>

template<>
class TexDimensionSpecificFunc<2>
{
    public:
        /**
         * \brief Wraps [glTextureSubImage2D()](https://docs.gl/gl4/glTexSubImage2D).
         *
         * \param textureId - rendererId of referenced OpenGL texture.
         */
        void setTexImageInTarget(GLuint textureId, std::shared_ptr<TextureData> textureData);
        /**
         * \brief Wraps [glTextureStorage2D()](https://docs.gl/gl4/glTexStorage2D).
         *
         * \param textureId - rendererId of referenced OpenGL texture.
         */
        void setTexStorageFormat(GLuint textureId, const std::shared_ptr<TextureData>& textureData);

};  // class TexDimensionSpecificFunc<2>

template<>
class TexDimensionSpecificFunc<3>
{
    public:
        /**
         * \brief Wraps [glTextureSubImage3D()](https://docs.gl/gl4/glTexSubImage3D).
         *
         * \param textureId - rendererId of referenced OpenGL texture.
         */
        void setTexImageInTarget(GLuint textureId, std::shared_ptr<TextureData> textureData);
        /**
         * \brief Wraps [glTextureStorage3D()](https://docs.gl/gl4/glTexStorage3D).
         *
         * \param textureId - rendererId of referenced OpenGL texture.
         */
        void setTexStorageFormat(GLuint textureId, const std::shared_ptr<TextureData>& textureData);

};  // class TexDimensionSpecificFunc<3>

/**
 * \brief Impl contains private data and methods of Texture.
 *
 * \param DimensionsNumber - the integer value in the range [1, 3], which specifies a number of dimensions in the
 * texture.
 */
template<size_t DimensionsNumber>
class Texture<DimensionsNumber>::Impl : public BaseTexture::BaseImpl
{
    public:
        /**
         * \brief Binds a texture to a target.
         *
         * Wraps [glBindTexture()](https://docs.gl/gl4/glBindTexture).
         * It is necessary for ogls::oglCore::bindForAMomentAndExecute().
         *
         * \param target    - the target to which the target object is bound (in other words, type of the texture).
         * \param textureId - rendererId of the texture, which must be bound.
         */
        static void                 bindToTarget(TextureTarget target, GLuint textureId);
        /**
         * \brief Returns the corresponding binding target for passed texture target.
         *
         * It is necessary for ogls::oglCore::bindForAMomentAndExecute().
         *
         * \param target - the target, binding target of which is needed.
         */
        static TextureBindingTarget getTargetAssociatedGetParameter(TextureTarget target) noexcept;

        /**
         * \see BaseTexture::BaseImpl::genTexture().
         */
        explicit Impl(TextureTarget target);
        /**
         * \see BaseTexture::BaseImpl::genTexture(), setData().
         */
        Impl(const Impl& obj);

        Impl& operator=(const Impl&)     = delete;
        Impl& operator=(Impl&&) noexcept = delete;

        /**
         * \see bindToTarget().
         */
        void bind() const;
        /**
         * \brief Sets new texture data and loads it in OpenGL texture.
         *
         * Wraps [glTextureStorage1D()](https://docs.gl/gl4/glTexStorage1D)
         * ([glTextureStorage2D()](https://docs.gl/gl4/glTexStorage2D),
         * [glTextureStorage3D()](https://docs.gl/gl4/glTexStorage3D)),
         * [glTextureSubImage1D()](https://docs.gl/gl4/glTexSubImage1D)
         * ([glTextureSubImage2D()](https://docs.gl/gl4/glTexSubImage2D),
         * [glTextureSubImage3D()](https://docs.gl/gl4/glTexSubImage3D))
         * and [glGenerateTextureMipmap()](https://docs.gl/gl4/glGenerateMipmap).
         *
         * \param textureData - data, which must be set in OpenGL texture.
         * \see specifyTextureStorageFormat().
         */
        void setData(std::shared_ptr<TextureData> textureData);
        /**
         * \brief Wraps [glTextureStorage1D()](https://docs.gl/gl4/glTexStorage1D),
         * [glTextureStorage2D()](https://docs.gl/gl4/glTexStorage2D) and
         * [glTextureStorage3D()](https://docs.gl/gl4/glTexStorage3D).
         *
         * \param textureData - texture data to specify parameters of texture storage.
         */
        void specifyTextureStorageFormat(const std::shared_ptr<TextureData>& textureData);

    public:
        /**
         * \brief The data of the texture.
         */
        std::shared_ptr<TextureData>               data;
        /**
         * \brief The integer value in the range [1, 3], which specifies a number of dimensions in the texture.
         */
        const GLuint                               dimensionsNumber         = {DimensionsNumber};
        /**
         * \brief Field to indicate, if storage format of the texture has been already set.
         */
        mutable bool                               isStorageFormatSpecified = false;
        /**
         * \brief Field to call some OpenGL functions, which are specific for dimension.
         */
        TexDimensionSpecificFunc<DimensionsNumber> specific;


        template<OpenGLBindableObject Type>
        friend void ogls::oglCore::bindForAMomentAndExecute(const Type&, const std::function<void()>&);

};  // class Texture<DimensionsNumber>::Impl

}  // namespace ogls::oglCore::texture

#endif
