#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>
#include <type_traits>

#include "helpers/macros.h"
#include "textureTypes.h"

/**
 * \brief texture namespace contains types, related to OpenGL textures.  
 */
namespace texture
{
    template<unsigned int DimensionsNumber>
    struct TexDimensionSpecificTypesAndFunc
    {
    };

    template<>
    struct TexDimensionSpecificTypesAndFunc<1>
    {
        TexImage1DTarget texImageTarget;
    };

    template<>
    struct TexDimensionSpecificTypesAndFunc<2>
    {
        TexImage2DTarget texImageTarget;
    };

    template<>
    struct TexDimensionSpecificTypesAndFunc<3>
    {
        TexImage3DTarget texImageTarget;
    };

	/**
	 * \brief Texture is a wrapper over OpenGL texture.
	 */
    template<unsigned int DimensionsNumber>
	class Texture
	{
        static_assert(DimensionsNumber >= 1 && DimensionsNumber <= 3,
            "The number of dimensions must be in range [1, 3].");

        // private:
            

		public:
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

            template<typename>
            void setData(decltype(m_dimensionTypesAndFunc.texImageTarget) target, std::shared_ptr<TextureData> textureData)
            {
                setTextureDataInTarget(target, std::move(textureData));
            }

			/*template
			void setParameter*/

			std::shared_ptr<TextureData> getData() const noexcept;

        private:
            void genTexture();
            void setTextureDataInTarget(TexImage1DTarget target, std::shared_ptr<TextureData> textureData);
            void setTextureDataInTarget(TexImage2DTarget target, std::shared_ptr<TextureData> textureData);
            void setTextureDataInTarget(TexImage3DTarget target, std::shared_ptr<TextureData> textureData);

		private:
			/**
			 * \brief Id of referenced OpenGL texture.
			 */
			unsigned int m_rendererId = 0;
            TextureTarget m_target = TextureTarget::TEXTURE_2D;
			std::shared_ptr<TextureData> m_data;
            bool m_isDataSet = false;
            TexDimensionSpecificTypesAndFunc<DimensionsNumber> m_dimensionTypesAndFunc;
            const unsigned int m_dimensionsNumber = DimensionsNumber;

	};
}

#endif
