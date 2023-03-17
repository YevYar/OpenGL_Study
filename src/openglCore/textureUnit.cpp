#include "textureUnit.h"

#include <format>
#include <stdexcept>

#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"
#include "openglLimits.h"
#include "texture.h"
#include "textureImpl.h"

using namespace texture;

namespace
{
	GLuint getActiveTextureUnitIndex()
	{
		return helpers::getOpenGLIntegerValue(GL_ACTIVE_TEXTURE) - GL_TEXTURE0;
	}

	void activateTextureUnitWithoutCheck(GLuint index)
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + index));
	}
}

struct TextureUnit::Impl
{
	public:
		explicit Impl(GLuint index) : m_index{ index }
		{
		}

	public:
		const GLuint m_index = 0;
		std::map<TextureTarget, std::shared_ptr<BaseTexture>> m_unitTextures;

};

namespace texture::TextureUnitsManager
{
	namespace
	{
		std::map<GLuint, std::shared_ptr<TextureUnit>> s_textureUnits;

		void checkTextureUnitIndexAndThrowIfNot(GLuint textureUnitIndex)
		{
			if (!checkIsValidTextureUnitIndex(textureUnitIndex))
			{
				const auto maxTUnitIndex = getOpenglLimit(LimitName::MAX_COMBINED_TEXTURE_IMAGE_UNITS);
				const auto errorMessage = std::format("Texture unit index must be less than {}.", maxTUnitIndex);
				throw std::out_of_range(errorMessage);
			}
		}		
	}
	
	std::shared_ptr<TextureUnit> get(GLuint index)
	{
		checkTextureUnitIndexAndThrowIfNot(index);

		if (s_textureUnits.contains(index))
		{
			return s_textureUnits.at(index);
		}

		auto tUnit = std::shared_ptr<TextureUnit>(new TextureUnit(index));
		s_textureUnits.insert({ index, tUnit });
		return tUnit;
	}

	void TextureUnitsManager::activateTextureUnit(GLuint index)
	{
		checkTextureUnitIndexAndThrowIfNot(index);

		if (index == getActiveTextureUnitIndex())
		{
			return;
		}

		activateTextureUnitWithoutCheck(index);
	}

	void TextureUnitsManager::activateTextureUnit(const std::shared_ptr<TextureUnit>& textureUnit)
	{
		if (textureUnit->m_impl->m_index == getActiveTextureUnitIndex())
		{
			return;
		}

		activateTextureUnitWithoutCheck(textureUnit->m_impl->m_index);
	}

	std::shared_ptr<TextureUnit> TextureUnitsManager::getActiveTextureUnit()
	{
		const auto activeTextureUnitIndex = getActiveTextureUnitIndex();
		return get(activeTextureUnitIndex);
	}
}

TextureUnit::TextureUnit(GLuint index) : m_impl{ std::make_unique<Impl>(index) }
{
}

TextureUnit::~TextureUnit() = default;

void TextureUnit::setTexture(const std::shared_ptr<BaseTexture>& texture)
{
    if (m_impl->m_unitTextures.contains(texture->m_impl->m_target) &&
        m_impl->m_unitTextures.at(texture->m_impl->m_target) == texture)
    {
        return;
    }

	const auto activeTextureUnitIndex = getActiveTextureUnitIndex();

	GLCall(glBindTextureUnit(m_impl->m_index, texture->m_impl->m_rendererId));
    m_impl->m_unitTextures.insert_or_assign(texture->m_impl->m_target, texture);

	activateTextureUnitWithoutCheck(activeTextureUnitIndex);
}

void TextureUnit::setTextures(const std::vector<std::shared_ptr<BaseTexture>>& textures)
{
    const auto activeTextureUnitIndex = getActiveTextureUnitIndex();

    for (const auto& texture : textures)
    {
        if (m_impl->m_unitTextures.contains(texture->m_impl->m_target) &&
            m_impl->m_unitTextures.at(texture->m_impl->m_target) == texture)
        {
            continue;
        }

        GLCall(glBindTextureUnit(m_impl->m_index, texture->m_impl->m_rendererId));
        m_impl->m_unitTextures.insert_or_assign(texture->m_impl->m_target, texture);
    }    

    activateTextureUnitWithoutCheck(activeTextureUnitIndex);
}

GLuint TextureUnit::getIndex() const noexcept
{
	return m_impl->m_index;
}

std::shared_ptr<BaseTexture> TextureUnit::getTexture(TextureTarget textureTarget) const noexcept
{
	return m_impl->m_unitTextures.contains(textureTarget) ? m_impl->m_unitTextures.at(textureTarget) : nullptr;
}

const std::map<TextureTarget, std::shared_ptr<BaseTexture>>& TextureUnit::getAllTextures() const noexcept
{
	return m_impl->m_unitTextures;
}

bool texture::checkIsValidTextureUnitIndex(GLuint textureUnitIndex) noexcept
{
	return textureUnitIndex <= getOpenglLimit(LimitName::MAX_COMBINED_TEXTURE_IMAGE_UNITS) - 1;
}

void texture::applyTexturesConfiguration(const TexturesConfiguration& texturesConfiguration)
{
    for (const auto& tUnit : texturesConfiguration)
    {
        texture::TextureUnitsManager::get(tUnit.first)->setTextures(tUnit.second);
    }
}
