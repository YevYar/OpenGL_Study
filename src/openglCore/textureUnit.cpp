#include "textureUnit.h"

#include <format>
#include <stdexcept>

#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"
#include "openglLimits.h"
#include "textureImpl.h"

namespace ogls::oglCore::texture
{
namespace
{
    void   activateTextureUnitWithoutCheck(GLuint index);
    GLuint getActiveTextureUnitIndex();

}  // namespace

class TextureUnit::Impl
{
    public:
        explicit Impl(GLuint i) noexcept : index{i}
        {
        }

    public:
        const GLuint                                          index = {0};
        std::map<TextureTarget, std::shared_ptr<BaseTexture>> unitTextures;

};  // class TextureUnit::Impl

namespace TextureUnitsManager
{
    namespace
    {
        void checkTextureUnitIndexAndThrowIfNot(GLuint textureUnitIndex);


        std::map<GLuint, std::shared_ptr<TextureUnit>> textureUnits;

    }  // namespace

    void activateTextureUnit(GLuint index)
    {
        checkTextureUnitIndexAndThrowIfNot(index);

        if (index == getActiveTextureUnitIndex())
        {
            return;
        }

        activateTextureUnitWithoutCheck(index);
    }

    void activateTextureUnit(const std::shared_ptr<TextureUnit>& textureUnit)
    {
        if (textureUnit->m_impl->index == getActiveTextureUnitIndex())
        {
            return;
        }

        activateTextureUnitWithoutCheck(textureUnit->m_impl->index);
    }

    std::shared_ptr<TextureUnit> get(GLuint index)
    {
        checkTextureUnitIndexAndThrowIfNot(index);

        if (textureUnits.contains(index))
        {
            return textureUnits.at(index);
        }

        auto tUnit = std::shared_ptr<TextureUnit>{new TextureUnit{index}};
        textureUnits.insert({index, tUnit});
        return tUnit;
    }

    std::shared_ptr<TextureUnit> getActiveTextureUnit()
    {
        const auto activeTextureUnitIndex = getActiveTextureUnitIndex();
        return get(activeTextureUnitIndex);
    }

    namespace
    {
        void checkTextureUnitIndexAndThrowIfNot(GLuint textureUnitIndex)
        {
            if (!checkIsValidTextureUnitIndex(textureUnitIndex))
            {
                const auto maxTUnitIndex = getOpenglLimit(LimitName::MaxCombinedTextureImageUnits);
                const auto errorMessage  = std::format("Texture unit index must be less than {}.", maxTUnitIndex);
                throw std::out_of_range{errorMessage};
            }
        }

    }  // namespace

}  // namespace TextureUnitsManager

TextureUnit::TextureUnit(GLuint index) : m_impl{std::make_unique<Impl>(index)}
{
}

TextureUnit::~TextureUnit() noexcept = default;

const std::map<TextureTarget, std::shared_ptr<BaseTexture>>& TextureUnit::getAllTextures() const noexcept
{
    return m_impl->unitTextures;
}

GLuint TextureUnit::getIndex() const noexcept
{
    return m_impl->index;
}

std::shared_ptr<BaseTexture> TextureUnit::getTexture(TextureTarget textureTarget) const noexcept
{
    return m_impl->unitTextures.contains(textureTarget) ? m_impl->unitTextures.at(textureTarget) : nullptr;
}

void TextureUnit::setTexture(const std::shared_ptr<BaseTexture>& texture)
{
    if (m_impl->unitTextures.contains(texture->m_impl->target)
        && m_impl->unitTextures.at(texture->m_impl->target) == texture)
    {
        return;
    }

    const auto activeTextureUnitIndex = getActiveTextureUnitIndex();

    OGLS_GLCall(glBindTextureUnit(m_impl->index, texture->m_impl->rendererId));
    m_impl->unitTextures.insert_or_assign(texture->m_impl->target, texture);

    // TODO: Check, if it is necessary to reactivate active texture unit after glBindTextureUnit()
    activateTextureUnitWithoutCheck(activeTextureUnitIndex);
}

void TextureUnit::setTextures(const std::vector<std::shared_ptr<BaseTexture>>& textures)
{
    const auto activeTextureUnitIndex = getActiveTextureUnitIndex();

    for (const auto& texture : textures)
    {
        if (m_impl->unitTextures.contains(texture->m_impl->target)
            && m_impl->unitTextures.at(texture->m_impl->target) == texture)
        {
            continue;
        }

        OGLS_GLCall(glBindTextureUnit(m_impl->index, texture->m_impl->rendererId));
        m_impl->unitTextures.insert_or_assign(texture->m_impl->target, texture);
    }

    // TODO: Check, if it is necessary to reactivate active texture unit after glBindTextureUnit()
    activateTextureUnitWithoutCheck(activeTextureUnitIndex);
}

void applyTexturesConfiguration(const TexturesConfiguration& texturesConfiguration)
{
    for (const auto& tUnit : texturesConfiguration)
    {
        TextureUnitsManager::get(tUnit.first)->setTextures(tUnit.second);
    }
}

bool checkIsValidTextureUnitIndex(GLuint textureUnitIndex)
{
    return textureUnitIndex <= getOpenglLimit(LimitName::MaxCombinedTextureImageUnits) - 1;
}

namespace
{
    void activateTextureUnitWithoutCheck(GLuint index)
    {
        OGLS_GLCall(glActiveTexture(GL_TEXTURE0 + index));
    }

    GLuint getActiveTextureUnitIndex()
    {
        return helpers::getOpenGLIntegerValue(GL_ACTIVE_TEXTURE) - GL_TEXTURE0;
    }

}  // namespace

}  // namespace ogls::oglCore::texture
