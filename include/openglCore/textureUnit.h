#ifndef OGLS_OGLCORE_TEXTURE_TEXTURE_UNIT_H
#define OGLS_OGLCORE_TEXTURE_TEXTURE_UNIT_H

#include <map>
#include <memory>
#include <vector>

#include <glad/glad.h>

#include "helpers/macros.h"
#include "texture.h"

namespace ogls::oglCore::texture
{
/**
 * \brief TexturesConfiguration is a map, where key is an index of texture unit and value is a vector of textures,
 * which are bound to this texture unit.
 */
using TexturesConfiguration = std::map<GLuint, std::vector<std::shared_ptr<BaseTexture>>>;

class TextureUnit;

/**
 * \brief TextureUnitsManager namespace contains functions to manage texture units.
 */
namespace TextureUnitsManager
{
    /**
     * \brief Wraps [glActiveTexture()](https://docs.gl/gl4/glActiveTexture).
     *
     * Index is checked to be valid (to be less than GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS).
     *
     * \param index - index of the texture unit to be activated.
     * \throw std::out_of_range.
     */
    void                         activateTextureUnit(GLuint index);
    /**
     * \brief Wraps [glActiveTexture()](https://docs.gl/gl4/glActiveTexture).
     *
     * \param textureUnit - texture unit to be activated.
     */
    void                         activateTextureUnit(const std::shared_ptr<TextureUnit>& textureUnit);
    /**
     * \brief Returns a TextureUnit object for passed index.
     *
     * Index is checked to be valid (to be less than GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS).
     *
     * TextureUnit object is created on first call.
     *
     * \param index - index of the texture unit to get.
     * \return a TextureUnit object for passed index.
     * \throw std::out_of_range.
     */
    std::shared_ptr<TextureUnit> get(GLuint index);
    /**
     * \brief Returns a TextureUnit object for current active texture unit.
     *
     * \see get().
     */
    std::shared_ptr<TextureUnit> getActiveTextureUnit();

};  // namespace TextureUnitsManager

/**
 * \brief TextureUnit is a wrapper over OpenGL texture unit.
 *
 * Objects of this class can be created only via TextureUnitsManager functions. It prevents unmanageable creation of
 * TextureUnit objects, because in OpenGL texture unit is not an object, which can be created by request. In OpenGL
 * texture units are presented by default.
 *
 * Read in details [Texture image units](https://www.khronos.org/opengl/wiki/Texture#Texture_image_units).
 */
class TextureUnit
{
    private:
        /**
         * \brief Impl contains private data and methods of TextureUnit.
         */
        struct Impl;

    public:
        TextureUnit() = delete;
        OGLS_NOT_COPYABLE_MOVABLE(TextureUnit)
        ~TextureUnit() noexcept;

        /**
         * \brief Returns all textures, which were bound to this texture unit via setTexture() or setTextures().
         *
         * \see setTexture(), setTextures().
         * \return all textures, which were bound to this texture unit.
         */
        const std::map<TextureTarget, std::shared_ptr<BaseTexture>>& getAllTextures() const noexcept;
        /**
         * \brief Returns an index of the texture unit.
         *
         * \return an index of the texture unit.
         */
        GLuint                                                       getIndex() const noexcept;
        /**
         * \brief Returns the texture, which was bound to passed target.
         *
         * \see setTexture(), setTextures().
         * \return the texture, which was bound to passed target, or nullptr.
         */
        std::shared_ptr<BaseTexture> getTexture(TextureTarget textureTarget) const noexcept;
        /**
         * \brief Wraps [glBindTextureUnit()](https://docs.gl/gl4/glBindTextureUnit).
         *
         * Previous active texture unit is not affected, this texture unit is not activated.
         *
         * \param texture - texture to bound.
         */
        void                         setTexture(const std::shared_ptr<BaseTexture>& texture);
        /**
         * \brief Wraps [glBindTextureUnit()](https://docs.gl/gl4/glBindTextureUnit).
         *
         * The same as setTexture(), but for several textures.
         *
         * \param textures - textures to bound.
         * \see setTexture().
         */
        void                         setTextures(const std::vector<std::shared_ptr<BaseTexture>>& textures);

    private:
        /**
         * \brief Constructs new TextureUnit object to wrap specified by index OpenGL texture unit.
         */
        explicit TextureUnit(GLuint index);

    private:
        /**
         * \brief Pointer to implementation.
         */
        std::unique_ptr<Impl> m_impl;


        friend void TextureUnitsManager::activateTextureUnit(const std::shared_ptr<TextureUnit>&);
        friend std::shared_ptr<TextureUnit> TextureUnitsManager::get(GLuint);

};  // class TextureUnit

/**
 * \brief Binds textures to texture units.
 *
 * \param texturesConfiguration - configuration to provide information about which textures must be bound to which
 * texture units.
 * \see TextureUnitsManager::get(), TextureUnit::setTextures().
 */
void applyTexturesConfiguration(const TexturesConfiguration& texturesConfiguration);

/**
 * \brief Casts a BaseTexture object to Texture<DimensionsNumber> object.
 *
 * \param DimensionsNumber - the integer value in the range [1, 3], which specifies a number of dimensions in the
 * texture.
 * \param textureUnitIndex - an index of texture unit to get texture from.
 * \param textureTarget    - a texture target to which the needed texture is bound.
 * \return casted object or nullptr.
 */
template<unsigned int DimensionsNumber>
auto castBaseTextureToTexture(GLuint textureUnitIndex, TextureTarget textureTarget)
{
    const auto tUnit = TextureUnitsManager::get(textureUnitIndex);
    auto       txt   = tUnit->getTexture(textureTarget);
    return txt ? std::dynamic_pointer_cast<Texture<DimensionsNumber>>(txt) : nullptr;
}

/**
 * \brief Casts a BaseTexture object to Texture<DimensionsNumber> object.
 *
 * \param DimensionsNumber - the integer value in the range [1, 3], which specifies a number of dimensions in the
 * texture.
 * \param textureUnit      - a texture unit to get texture from.
 * \param textureTarget    - a texture target to which the needed texture is bound.
 * \return casted object or nullptr.
 */
template<unsigned int DimensionsNumber>
auto castBaseTextureToTexture(const std::shared_ptr<TextureUnit>& textureUnit, TextureTarget textureTarget) noexcept
{
    auto txt = textureUnit->getTexture(textureTarget);
    return txt ? std::dynamic_pointer_cast<Texture<DimensionsNumber>>(txt) : nullptr;
}

/**
 * \brief Casts a BaseTexture object to Texture<DimensionsNumber> object.
 *
 * \param DimensionsNumber - the integer value in the range [1, 3], which specifies a number of dimensions in the
 * texture.
 * \param texturesConfig   - configuration to provide information about which textures must be bound to which
 * texture units.
 * \param textureUnitIndex - an index of texture unit to get texture from.
 * \param textureIndex     - an index of needed texture in the vector of texture unit textures.
 * \return casted object.
 * \throw std::out_of_range.
 */
template<unsigned int DimensionsNumber>
auto castBaseTextureToTexture(const TexturesConfiguration& texturesConfig, GLuint textureUnitIndex, size_t textureIndex)
{
    return std::dynamic_pointer_cast<Texture<DimensionsNumber>>(texturesConfig.at(textureUnitIndex).at(textureIndex));
}

/**
 * \brief Checks validity of the index of texture unit.
 *
 * Index is valid if it is less than GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS.
 *
 * \param textureUnitIndex - index of texture unit to check.
 * \return true if index is valid, false otherwise.
 */
bool checkIsValidTextureUnitIndex(GLuint textureUnitIndex);

}  // namespace ogls::oglCore::texture

#endif
