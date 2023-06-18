#ifndef OGLS_OGLCORE_TEXTURE_TEXTURE_H
#define OGLS_OGLCORE_TEXTURE_TEXTURE_H

#include <memory>

#include "generalTypes.h"
#include "textureTypes.h"

/**
 * \namespace ogls::oglCore::texture
 * \brief texture namespace contains types, related to OpenGL textures.
 */
namespace ogls::oglCore::texture
{
/**
 * \brief BaseTexture is a base class for texture classes.
 *
 * It is responsible for creation and deletion of a texture in OpenGL state machine.
 */
class BaseTexture : public ICloneable
{
    protected:
        /**
         * \brief BaseImpl contains private data and methods of BaseTexture.
         */
        class BaseImpl;

    public:
        OGLS_DEFAULT_MOVABLE(BaseTexture)
        BaseTexture() = delete;
        /**
         * \brief Deletes the texture in OpenGL state machine.
         *
         * Wraps [glDeleteTextures()](https://docs.gl/gl4/glDeleteTextures).
         */
        virtual ~BaseTexture() noexcept;

        BaseTexture& operator=(const BaseTexture& obj) = delete;

        BaseTexture* clone() const override;

    protected:
        /**
         * \brief Constructs new BaseTexture object using pointer to BaseImpl object or to object of the class,
         * derived from BaseImpl.
         *
         * A BaseImpl object or an object of the class, derived from BaseImpl, is responsible for generating of
         * a texture object in OpenGL state machine.
         *
         * \param impl - pointer to BaseImpl object or to object of the class, derived from BaseImpl.
         */
        explicit BaseTexture(std::unique_ptr<BaseImpl> impl) noexcept;
        /**
         * \brief Constructs new BaseTexture as copy of other BaseTexture.
         *
         * However new 1 texture in OpenGL state machine is generated.
         *
         * Wraps [glCreateTextures()](https://docs.gl/gl4/glCreateTextures).
         *
         * \throw ogls::exceptions::GLRecAcquisitionException().
         */
        BaseTexture(const BaseTexture& obj);

    protected:
        /**
         * \brief Pointer to implementation.
         */
        std::unique_ptr<BaseImpl> m_impl;


        friend class TextureUnit;

};  // class BaseTexture

/**
 * \brief Texture is a wrapper over OpenGL texture.
 *
 * Read in details [Texture](https://www.khronos.org/opengl/wiki/Texture).
 *
 * \param DimensionsNumber - the integer value in the range [1, 3], which specifies a number of dimensions in the
 * texture.
 */
template<unsigned int DimensionsNumber>
class Texture : public BaseTexture
{
        static_assert(DimensionsNumber >= 1 && DimensionsNumber <= 3,
                      "The number of dimensions must be in range [1, 3].");


    private:
        /**
         * \brief Impl contains private data and methods of Texture.
         */
        class Impl;

    public:
        /**
         * \brief Unbinds current texture from the target.
         *
         * Wraps [glBindTexture()](https://docs.gl/gl4/glBindTexture).
         *
         * \param target - target texture to be unbound from.
         */
        static void unbindTarget(TextureTarget target);

        OGLS_DEFAULT_MOVABLE(Texture)
        /**
         * \brief Constructs new Texture object and generates new 1 texture in OpenGL state machine.
         *
         * Wraps [glCreateTextures()](https://docs.gl/gl4/glCreateTextures).
         *
         * \param target - target to bind texture to (in other words, type of the texture).
         * \throw ogls::exceptions::GLRecAcquisitionException(), std::invalid_argument.
         */
        explicit Texture(TextureTarget target);
        /**
         * \brief Constructs new Texture object with passed arguments, generates new 1 texture in OpenGL state machine
         * and loads the data in referenced OpenGL texture object.
         *
         * Wraps [glCreateTextures()](https://docs.gl/gl4/glCreateTextures).
         *
         * \param target      - target to bind texture to (in other words, type of the texture).
         * \param textureData - the data of the texture to be load in OpenGL texture.
         * \see setData().
         * \throw ogls::exceptions::GLRecAcquisitionException(), std::invalid_argument.
         */
        Texture(TextureTarget target, std::shared_ptr<TextureData> textureData);
        /**
         * \brief Deletes the texture in OpenGL state machine.
         *
         * \see BaseTexture::~BaseTexture().
         */
        ~Texture() noexcept;

        Texture& operator=(const Texture& obj) = delete;

        /**
         * \brief Wraps [glBindTexture()](https://docs.gl/gl4/glBindTexture).
         */
        void                         bind() const;
        /**
         * \brief Returns data of the Texture.
         *
         * \return data of the Texture.
         */
        std::shared_ptr<TextureData> getData() const noexcept;
        /**
         * \brief Returns target (type) of the Texture.
         *
         * \return target (type) of the Texture.
         */
        TextureTarget                getTarget() const noexcept;
        /**
         * \brief Sets new Texture data and loads it in OpenGL texture.
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
        void                         setData(std::shared_ptr<TextureData> textureData);
        /**
         * \brief Wraps [glTextureParameterf()](https://docs.gl/gl4/glTexParameter) and
         * [glTextureParameteri()](https://docs.gl/gl4/glTexParameter).
         *
         * \param Type      - one of type GLfloat or GLint.
         * \param parameter - 'pname' (the symbolic name of a single-valued texture parameter).
         * \param value     - the value to set.
         */
        template<typename Type>
        requires std::is_same_v<GLfloat, Type> || std::is_same_v<GLint, Type>
        void setParameter(TexParameterName parameter, Type value);
        /**
         * \brief Wraps [glTextureParameterIiv()](https://docs.gl/gl4/glTexParameter) and
         * [glTextureParameterIuiv()](https://docs.gl/gl4/glTexParameter).
         *
         * \param Type      - one of type GLuint or GLint.
         * \param parameter - 'pname' (the symbolic name of a single-valued texture parameter).
         * \param values    - the values to set.
         */
        template<typename Type>
        requires std::is_same_v<GLint, Type> || std::is_same_v<GLuint, Type>
        void setParameterIV(TexParameterName parameter, const Type* values);
        /**
         * \brief Wraps [glTextureParameterfv()](https://docs.gl/gl4/glTexParameter) and
         * [glTextureParameteriv()](https://docs.gl/gl4/glTexParameter).
         *
         * \param Type      - one of type GLfloat or GLint.
         * \param parameter - 'pname' (the symbolic name of a single-valued texture parameter).
         * \param values    - the values to set.
         */
        template<typename Type>
        requires std::is_same_v<GLfloat, Type> || std::is_same_v<GLint, Type>
        void setParameterV(TexParameterName parameter, const Type* values);
        /**
         * \brief Wraps [glTextureStorage1D()](https://docs.gl/gl4/glTexStorage1D),
         * [glTextureStorage2D()](https://docs.gl/gl4/glTexStorage2D) and
         * [glTextureStorage3D()](https://docs.gl/gl4/glTexStorage3D).
         *
         * \param textureData - texture data to specify parameters of texture storage.
         */
        void specifyTextureStorageFormat(const std::shared_ptr<TextureData>& textureData);
        /**
         * \brief Calls unbindTarget() with the target of the texture.
         */
        void unbind() const;

        Texture* clone() const override;

    private:
        /**
         * \brief Constructs new Texture as copy of other Texture.
         *
         * However new 1 texture in OpenGL state machine is generated.
         *
         * Wraps [glCreateTextures()](https://docs.gl/gl4/glCreateTextures).
         *
         * \see setData().
         * \throw ogls::exceptions::GLRecAcquisitionException().
         */
        Texture(const Texture& obj);

        /**
         * \brief Returns a pointer to an object of implementation class, casted from BaseTexture::BaseImpl to Impl.
         */
        Impl* impl() const noexcept;

};  // class Texture

/**
 * \brief Casts a BaseTexture object to Texture<DimensionsNumber> object.
 *
 * \param DimensionsNumber - the integer value in the range [1, 3], which specifies a number of dimensions in the
 * texture.
 * \param baseTexture      - an object to cast to Texture<DimensionsNumber>.
 */
template<unsigned int DimensionsNumber>
auto castBaseTextureToTexture(std::shared_ptr<BaseTexture> baseTexture) noexcept
{
    return std::dynamic_pointer_cast<Texture<DimensionsNumber>>(baseTexture);
}

}  // namespace ogls::oglCore::texture

#endif
