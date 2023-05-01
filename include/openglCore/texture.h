#ifndef OGLS_OGLCORE_TEXTURE_TEXTURE_H
#define OGLS_OGLCORE_TEXTURE_TEXTURE_H

#include <memory>

#include "generalTypes.h"
#include "textureTypes.h"

/**
 * \namespace texture
 * \brief texture namespace contains types, related to OpenGL textures.
 */
namespace ogls::oglCore::texture
{
class BaseTexture : public ICloneable
{
    protected:
        /**
         * \brief BaseImpl contains private data and methods of BaseTexture.
         */
        struct BaseImpl;

    public:
        OGLS_DEFAULT_MOVABLE(BaseTexture)
        BaseTexture();
        virtual ~BaseTexture();

        BaseTexture& operator=(const BaseTexture& obj) = delete;

        BaseTexture* clone() const override;

    protected:
        explicit BaseTexture(std::unique_ptr<BaseImpl> impl);
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
 */
template<unsigned int DimensionsNumber>
class Texture : public BaseTexture
{
        static_assert(DimensionsNumber >= 1 && DimensionsNumber <= 3,
                      "The number of dimensions must be in range [1, 3].");


    public:
        using TexImageTarget = TexDimensionSpecificTypes<DimensionsNumber>::TexImageTarget;

    private:
        /**
         * \brief Impl contains private data and methods of Texture.
         */
        struct Impl;

    public:
        /**
         * \brief Unbinds current texture from the target.
         *
         * \param target - target texture to be unbound from.
         */
        static void unbindTarget(TextureTarget target) noexcept;

        OGLS_DEFAULT_MOVABLE(Texture)
        explicit Texture(TextureTarget target);
        Texture(TextureTarget target, TexImageTarget texImageTarget, std::shared_ptr<TextureData> textureData);
        ~Texture();

        Texture& operator=(const Texture& obj) = delete;

        void                         bind() const noexcept;
        std::shared_ptr<TextureData> getData() const noexcept;
        TextureTarget                getTarget() const noexcept;
        void                         setData(TexImageTarget texImageTarget, std::shared_ptr<TextureData> textureData);
        template<typename Type>
        requires std::is_same_v<GLfloat, Type> || std::is_same_v<GLint, Type>
        void setParameter(TexParameterName parameter, Type value);
        template<typename Type>
        requires std::is_same_v<GLint, Type> || std::is_same_v<GLuint, Type>
        void setParameterIV(TexParameterName parameter, const Type* values);
        template<typename Type>
        requires std::is_same_v<GLfloat, Type> || std::is_same_v<GLint, Type>
        void setParameterV(TexParameterName parameter, const Type* values);
        void specifyTextureStorageFormat(const std::shared_ptr<TextureData>& textureData) const noexcept;
        void unbind() const noexcept;

        Texture* clone() const override;

    private:
        Texture(const Texture& obj);

        Impl* impl() const noexcept;

};  // class Texture

template<unsigned int DimensionsNumber>
auto castBaseTextureToTexture(std::shared_ptr<BaseTexture> baseTexture)
{
    return std::dynamic_pointer_cast<Texture<DimensionsNumber>>(baseTexture);
}

}  // namespace ogls::oglCore::texture

#endif
