#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>

#include "helpers/macros.h"
#include "textureTypes.h"

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

		public:
			Texture(TextureBindingTarget target, std::shared_ptr<TextureData> textureData);

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
			static void unbindTarget(TextureBindingTarget target) noexcept;

			void bind() const noexcept;

			void unbind() const noexcept;

			void setData(std::shared_ptr<TextureData> textureData);

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
            TextureBindingTarget m_target = TextureBindingTarget::TEXTURE_2D;
			std::shared_ptr<TextureData> m_data;
            bool m_isDataSet = false;

	};
}

#endif
