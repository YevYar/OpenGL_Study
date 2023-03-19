#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>

#include "generalTypes.h"
#include "helpers/macros.h"
#include "textureTypes.h"
#include "textureUnit.h"

/**
 * \brief texture namespace contains types, related to OpenGL textures.  
 */
namespace openglCore::texture
{
    class BaseTexture : public ICloneable
    {
        public:
            BaseTexture();

            DEFAULT_MOVABLE(BaseTexture)

            virtual ~BaseTexture();

            BaseTexture& operator=(const BaseTexture& obj) = delete;

            BaseTexture* clone() const override;

        protected:
            struct BaseImpl;
            std::unique_ptr<BaseImpl> m_impl;

        protected:
            explicit BaseTexture(std::unique_ptr<BaseImpl> impl);
            BaseTexture(const BaseTexture& obj);

        friend class TextureUnit;

    };

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

		public:
            explicit Texture(TextureTarget target);
            Texture(TextureTarget target, TexImageTarget texImageTarget, std::shared_ptr<TextureData> textureData);
            
            DEFAULT_MOVABLE(Texture)

			~Texture();

            Texture& operator=(const Texture& obj) = delete;
            
			/**
			 * \brief Unbinds current texture from the target.
			 *
			 * \param target - target texture to be unbound from.
			 */
			static void unbindTarget(TextureTarget target) noexcept;

            Texture* clone() const override;

			void bind() const noexcept;

			void unbind() const noexcept;

            void specifyTextureStorageFormat(const std::shared_ptr<TextureData>& textureData) const noexcept;
            void setData(TexImageTarget texImageTarget, std::shared_ptr<TextureData> textureData);

			template<typename Type>
            requires std::is_same_v<GLfloat, Type> || std::is_same_v<GLint, Type>
            void setParameter(TexParameterName parameter, Type value);

            template<typename Type>
            requires std::is_same_v<GLfloat, Type> || std::is_same_v<GLint, Type>
            void setParameterV(TexParameterName parameter, const Type* values);

            template<typename Type>
            requires std::is_same_v<GLint, Type> || std::is_same_v<GLuint, Type>
            void setParameterIV(TexParameterName parameter, const Type* values);

            TextureTarget getTarget() const noexcept;
			std::shared_ptr<TextureData> getData() const noexcept;

        private:
            Texture(const Texture& obj);

            struct Impl;
            Impl* impl() const noexcept;

	};

    template<unsigned int DimensionsNumber>
    auto castBaseTextureToTexture(
        std::shared_ptr<texture::BaseTexture> baseTexture)
    {
        return std::dynamic_pointer_cast<texture::Texture<DimensionsNumber>>(baseTexture);
    }

    template<unsigned int DimensionsNumber>
    auto castBaseTextureToTexture(GLuint textureUnitIndex, texture::TextureTarget textureTarget)
    {
        const auto tUnit = texture::TextureUnitsManager::get(textureUnitIndex);
        return std::dynamic_pointer_cast<texture::Texture<DimensionsNumber>>(tUnit->getTexture(textureTarget));
    }

    template<unsigned int DimensionsNumber>
    auto castBaseTextureToTexture(const std::shared_ptr<texture::TextureUnit>& textureUnit,
        texture::TextureTarget textureTarget)
    {
        return std::dynamic_pointer_cast<texture::Texture<DimensionsNumber>>(
            textureUnit->getTexture(textureTarget));
    }

    template<unsigned int DimensionsNumber>
    auto castBaseTextureToTexture(const texture::TexturesConfiguration& texturesConfig, GLuint textureUnitIndex,
        unsigned int textureIndex)
    {
        return std::dynamic_pointer_cast<texture::Texture<DimensionsNumber>>(
            texturesConfig.at(textureUnitIndex).at(textureIndex));
    }
}

#endif
