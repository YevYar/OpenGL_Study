#ifndef OGLS_HELPERS_HELPERS_H
#define OGLS_HELPERS_HELPERS_H

#include <memory>
#include <string>

#include "textureTypes.h"

/**
 * \namespace helpers
 * \brief helpers namespace contains some helper types and functions.
 */
namespace ogls::helpers
{
/**
 * \brief Properly frees memory, in which the texture was written.
 */
void freeTextureData(ogls::oglCore::texture::TextureData& textureData);

/**
 * \brief Opens the file and reads the content.
 *
 * \param pathToFile - a path to file to be read.
 * \return the srd::string with content of the file.
 * \throw ogls::exceptions::FileOpeningException(), ogls::exceptions::FileReadingException().
 */
std::string readStringFromFile(const std::string& pathToFile);

/**
 * \brief Opens the file and reads the content as texture image.
 *
 * \param pathToFile - a path to file to be read.
 * \return bytes of the image and accompanying information about the image.
 * \throw ogls::exceptions::FileOpeningException(), ogls::exceptions::FileReadingException().
 */
std::unique_ptr<ogls::oglCore::texture::TextureData> readTextureFromFile(const std::string& pathToFile);

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

}  // namespace ogls::helpers

#endif
