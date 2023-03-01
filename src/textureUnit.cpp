#include "textureUnit.h"

#include <format>
#include <stdexcept>

#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"
#include "openglLimits.h"

using namespace texture;

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

		GLCall(glActiveTexture(GL_TEXTURE0 + index));
	}

	void TextureUnitsManager::activateTextureUnit(const std::shared_ptr<TextureUnit>& textureUnit)
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + textureUnit->m_index));
	}

	std::shared_ptr<TextureUnit> TextureUnitsManager::getActiveTextureUnit()
	{
		const auto activeTextureUnitIndex = helpers::getOpenGLIntegerValue(GL_ACTIVE_TEXTURE) - GL_TEXTURE0;
		return get(activeTextureUnitIndex);
	}
}

bool texture::checkIsValidTextureUnitIndex(GLuint textureUnitIndex) noexcept
{
	return textureUnitIndex <= getOpenglLimit(LimitName::MAX_COMBINED_TEXTURE_IMAGE_UNITS) - 1;
}
