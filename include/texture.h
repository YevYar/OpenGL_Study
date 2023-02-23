#ifndef TEXTURE_H
#define TEXTURE_H

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
            Texture(TextureTarget target, TexImageTarget texImageTarget, std::shared_ptr<TextureData> textureData) :
                Texture(target)
            {
                setData(texImageTarget, std::move(textureData));
            }

            Texture(const Texture& obj);
            Texture(Texture&& obj) noexcept;

			~Texture();

            Texture& operator=(const Texture& obj);
            Texture& operator=(Texture&& obj) noexcept;

			/**
			 * \brief Unbinds current texture from the target.
			 *
			 * \param target - target texture to be unbound from.
			 */
			static void unbindTarget(TextureTarget target) noexcept;

			void bind() const noexcept;

			void unbind() const noexcept;

            void setData(TexImageTarget texImageTarget,
                std::shared_ptr<TextureData> textureData)
            {
                bind();
                m_dimensionTypesAndFunc.setTexImageInTarget(texImageTarget, textureData);
                m_data = std::move(textureData);
                m_lastTexImageTarget = texImageTarget;
            }

			/*template
			void setParameter*/

			std::shared_ptr<TextureData> getData() const noexcept;

        private:
            void genTexture();

		private:
			/**
			 * \brief Id of referenced OpenGL texture.
			 */
			unsigned int m_rendererId = 0;
            unsigned int m_dimensionsNumber = DimensionsNumber;
            TextureTarget m_target = TextureTarget::TEXTURE_2D;
			std::shared_ptr<TextureData> m_data;            
            TexImageTarget m_lastTexImageTarget;

	};
}

#endif
