#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>

#include "helpers/macros.h"

/**
 * \brief texture namespace contains types, related to OpenGL textures.  
 */
namespace texture
{
	/*
	 * \brief TextureTarget is a type of the texture, which represents 'target' parameter of 
	 * [glBindTexture()](https://docs.gl/gl4/glBindTexture). 
	 */
	enum class TextureTarget : unsigned int
	{
		TEXTURE_1D = 0x0DE0, TEXTURE_2D = 0x0DE1, TEXTURE_3D = 0x806F, TEXTURE_1D_ARRAY = 0x8C18,
		TEXTURE_2D_ARRAY = 0x8C1A, TEXTURE_RECTANGLE = 0x84F5, TEXTURE_CUBE_MAP = 0x8513,
		TEXTURE_CUBE_MAP_ARRAY = 0x9009, TEXTURE_BUFFER = 0x8C2A, TEXTURE_2D_MULTISAMPLE = 0x9100,
		TEXTURE_2D_MULTISAMPLE_ARRAY = 0x9102
	};

    class TextureData
    {
        public:
            TextureData(unsigned char* textureData, int width, int height, int nChannels);

            NOT_COPYABLE_MOVABLE(TextureData)

            ~TextureData();

            unsigned char* getData() const noexcept;

        private:
            unsigned char* m_data = nullptr;
            int m_height = 0;
            int m_width = 0;
            int m_nChannels = 0;

    };

	/**
	 * \brief Texture is a wrapper over OpenGL texture.
	 */
	class Texture
	{
		public:
			Texture(TextureTarget target);

			Texture(TextureTarget target, std::shared_ptr<TextureData> textureData);

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

			void setData(std::shared_ptr<TextureData> textureData);

			/*template
			void setParameter*/

			std::shared_ptr<TextureData> getData() const noexcept;

		private:
			/**
			 * \brief Id of referenced OpenGL texture.
			 */
			unsigned int m_rendererId = 0;
			TextureTarget m_target = TextureTarget::TEXTURE_2D;
			std::shared_ptr<TextureData> m_data;

	};
}

#endif
