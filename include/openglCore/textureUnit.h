#ifndef TEXTURE_UNIT_H
#define TEXTURE_UNIT_H

#include <glad/glad.h>
#include <map>
#include <memory>
#include <vector>

#include "helpers/macros.h"

namespace texture
{
    class BaseTexture;
    class TextureUnit;
    enum class TextureTarget : GLenum;

    using TexturesConfiguration = std::map<GLuint, std::vector<std::shared_ptr<BaseTexture>>>;

    namespace TextureUnitsManager
    {
        std::shared_ptr<TextureUnit> get(GLuint index);
        void activateTextureUnit(GLuint index);
        void activateTextureUnit(const std::shared_ptr<TextureUnit>& textureUnit);
        std::shared_ptr<TextureUnit> getActiveTextureUnit();
    };

    class TextureUnit
    {
        public:
            TextureUnit() = delete;
            NOT_COPYABLE_MOVABLE(TextureUnit)

            void setTexture(const std::shared_ptr<BaseTexture>& texture);
            void setTextures(const std::vector<std::shared_ptr<BaseTexture>>& textures);
            GLuint getIndex() const noexcept;
            std::shared_ptr<BaseTexture> getTexture(TextureTarget textureTarget) const noexcept;
            const std::map<TextureTarget, std::shared_ptr<BaseTexture>>& getAllTextures() const noexcept;

        private:
            TextureUnit(GLuint index);

        private:
            const GLuint m_index = 0;
            std::map<TextureTarget, std::shared_ptr<BaseTexture>> m_unitTextures;

        friend std::shared_ptr<TextureUnit> TextureUnitsManager::get(GLuint);
        friend void TextureUnitsManager::activateTextureUnit(const std::shared_ptr<TextureUnit>&);

    };

    bool checkIsValidTextureUnitIndex(GLuint textureUnitIndex) noexcept;
    void applyTexturesConfiguration(const TexturesConfiguration& texturesConfiguration);

}

#endif
