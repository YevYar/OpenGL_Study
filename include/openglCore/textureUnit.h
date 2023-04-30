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
using TexturesConfiguration = std::map<GLuint, std::vector<std::shared_ptr<BaseTexture>>>;

enum class TextureTarget : GLenum;
class TextureUnit;

namespace TextureUnitsManager
{
    void                         activateTextureUnit(GLuint index);
    void                         activateTextureUnit(const std::shared_ptr<TextureUnit>& textureUnit);
    std::shared_ptr<TextureUnit> get(GLuint index);
    std::shared_ptr<TextureUnit> getActiveTextureUnit();

};  // namespace TextureUnitsManager

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
        ~TextureUnit();

        const std::map<TextureTarget, std::shared_ptr<BaseTexture>>& getAllTextures() const noexcept;
        GLuint                                                       getIndex() const noexcept;
        std::shared_ptr<BaseTexture> getTexture(TextureTarget textureTarget) const noexcept;
        void                         setTexture(const std::shared_ptr<BaseTexture>& texture);
        void                         setTextures(const std::vector<std::shared_ptr<BaseTexture>>& textures);

    private:
        explicit TextureUnit(GLuint index);

    private:
        /**
         * \brief Pointer to implementation.
         */
        std::unique_ptr<Impl> m_impl;


        friend void TextureUnitsManager::activateTextureUnit(const std::shared_ptr<TextureUnit>&);
        friend std::shared_ptr<TextureUnit> TextureUnitsManager::get(GLuint);

};  // class TextureUnit

void applyTexturesConfiguration(const TexturesConfiguration& texturesConfiguration);
bool checkIsValidTextureUnitIndex(GLuint textureUnitIndex) noexcept;

}  // namespace ogls::oglCore::texture

#endif
