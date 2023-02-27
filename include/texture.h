#ifndef TEXTURE_H
#define TEXTURE_H

#include <functional>
#include <memory>

#include "textureImpl.h"

/**
 * \brief texture namespace contains types, related to OpenGL textures.  
 */
namespace texture
{
	/**
	 * \brief Texture is a wrapper over OpenGL texture.
	 */
    template<unsigned int DimensionsNumber>
	class Texture
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
            Texture(const Texture& obj);
            Texture(Texture&& obj) noexcept;

			~Texture();

            Texture& operator=(const Texture& obj) = delete;
            Texture& operator=(Texture&& obj) noexcept;

            static void bindTextureToTextureUnit(GLuint textureUnit, TextureTarget textureTarget,
                const Texture<DimensionsNumber>& texture);
            
			/**
			 * \brief Unbinds current texture from the target.
			 *
			 * \param target - target texture to be unbound from.
			 */
			static void unbindTarget(TextureTarget target) noexcept;

			void bind() const noexcept;

			void unbind() const noexcept;

            void setData(TexImageTarget texImageTarget, std::shared_ptr<TextureData> textureData);

			template<typename Type, typename = std::enable_if_t<std::is_same_v<GLfloat, Type> ||
                                                                std::is_same_v<GLint, Type>>>
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

            template<typename Type, typename = std::enable_if_t<std::is_same_v<GLfloat, Type> ||
                                                                std::is_same_v<GLint, Type>>>
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

            template<typename Type, typename = std::enable_if_t<std::is_same_v<GLint, Type> ||
                                                                std::is_same_v<GLuint, Type>>>
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

			std::shared_ptr<TextureData> getData() const noexcept;

        private:
            void genTexture();
            void bindForAMomentAndExecute(const std::function<void()>& funcToExecute = []() { });
            void deleteTexture() noexcept;

		private:
			/**
			 * \brief Id of referenced OpenGL texture.
			 */
			unsigned int m_rendererId = 0;
            const unsigned int m_dimensionsNumber = DimensionsNumber;
            TextureTarget m_target = TextureTarget::TEXTURE_2D;
			std::shared_ptr<TextureData> m_data;            
            TexImageTarget m_lastTexImageTarget;
            mutable bool m_isBound = false;

	};
}

#endif
