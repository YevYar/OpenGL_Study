#ifndef TEXTURE_H
#define TEXTURE_H

#include <functional>
#include <memory>

#include "generalTypes.h"
#include "helpers/macros.h"
#include "helpers/openglHelpers.h"
#include "textureImpl.h"
#include "textureUnit.h"

/**
 * \brief texture namespace contains types, related to OpenGL textures.  
 */
namespace texture
{
    class BaseTexture : public ICloneable
    {
        public:
            BaseTexture() = default;

            DEFAULT_COPYABLE_MOVABLE(BaseTexture)

            virtual ~BaseTexture() = default;

            BaseTexture* clone() const override;

        protected:
            BaseTexture(TextureTarget target);
            BaseTexture(GLuint rendererId, TextureTarget target);

        protected:
            /**
             * \brief Id of referenced OpenGL texture.
             */
            GLuint m_rendererId = 0;
            TextureTarget m_target = TextureTarget::TEXTURE_2D;

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

        private:
            TexDimensionSpecificTypesAndFunc<DimensionsNumber> m_dimensionTypesAndFunc;

        public:
            using TexImageTarget = decltype(m_dimensionTypesAndFunc.texImageTarget);

		public:
            Texture(TextureTarget target);
            Texture(TextureTarget target, TexImageTarget texImageTarget, std::shared_ptr<TextureData> textureData);
            Texture(Texture&& obj) noexcept;

			~Texture();

            Texture& operator=(const Texture& obj) = delete;
            Texture& operator=(Texture&& obj) noexcept;
            
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
            void setParameter(TexParameterName parameter, Type value)
            {
                if constexpr (std::is_same_v<GLfloat, Type>)
                {
                    GLCall(glTextureParameterf(m_rendererId, helpers::toUType(parameter), value));
                }
                else
                {
                    GLCall(glTextureParameteri(m_rendererId, helpers::toUType(parameter), value));
                }
            }

            template<typename Type>
            requires std::is_same_v<GLfloat, Type> || std::is_same_v<GLint, Type>
            void setParameterV(TexParameterName parameter, const Type* values)
            {
                if constexpr (std::is_same_v<GLfloat, Type>)
                {
                    GLCall(glTextureParameterfv(m_rendererId, helpers::toUType(parameter), values));
                }
                else
                {
                    GLCall(glTextureParameteriv(m_rendererId, helpers::toUType(parameter), values));
                }
            }

            template<typename Type>
            requires std::is_same_v<GLint, Type> || std::is_same_v<GLuint, Type>
            void setParameterIV(TexParameterName parameter, const Type* values)
            {
                if constexpr (std::is_same_v<GLint, Type>)
                {
                    GLCall(glTextureParameterIiv(m_rendererId, helpers::toUType(parameter), values));
                }
                else
                {
                    GLCall(glTextureParameterIuiv(m_rendererId, helpers::toUType(parameter), values));
                }
            }

            TextureTarget getTarget() const noexcept;
			std::shared_ptr<TextureData> getData() const noexcept;

        private:
            Texture(const Texture& obj);

            static void bindToTarget(TextureTarget target, GLuint textureId) noexcept;
            static TextureBindingTarget getTargetAssociatedGetParameter(TextureTarget target) noexcept;

            void genTexture();
            void deleteTexture() noexcept;

		private:			
            const unsigned int m_dimensionsNumber = DimensionsNumber;
			std::shared_ptr<TextureData> m_data;            
            TexImageTarget m_lastTexImageTarget;
            mutable bool m_isStorageFormatSpecified = false;

            template<typename Type>
            friend class helpers::OpenGLBindableObject;

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
