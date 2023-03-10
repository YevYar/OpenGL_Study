#ifndef HELPERS_H
#define HELPERS_H

#include <memory>
#include <string>

#include "textureTypes.h"

/**
 * \brief helpers namespace contains some helper types and functions.
 */
namespace helpers
{
    /**
     * \brief Opens the file and reads the content.
     * 
     * \param pathToFile - a path to file to be read.
     * \return the srd::string with content of the file.
     * \throw exceptions::FileOpeningException(), exceptions::FileReadingException().
     */
	std::string readStringFromFile(const std::string& pathToFile);

    std::unique_ptr<texture::TextureData> readTextureFromFile(const std::string& pathToFile);

    void freeTextureData(texture::TextureData& textureData);

    /**
     * \brief Casts an object to a type of elements of its enum type.
     * 
     * \param enumeration - object of enum type.
     * \return object, casted to the type of elements of its enum type.
     */
	constexpr auto toUType(auto enumeration) noexcept
	{
		return static_cast<std::underlying_type_t<decltype(enumeration)>>(enumeration);
	}

    /**
     * \brief Gets the integer value of the specified parameter from the OpenGL state machine.
     *
     * The list of available parameters see in description of
     * [glGet()](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGet.xhtml).
     *
     * \return the integer value of the specified parameter.
     */
    GLint getOpenGLIntegerValue(GLenum parameterName);
}

#endif
