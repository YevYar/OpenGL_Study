#ifndef HELPERS_H
#define HELPERS_H

#include <string>

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

    /**
     * \brief Casts an object to a type of elements of its enum type.
     * 
     * \param enumeration - object of enum type.
     * \return object, casted to the type of elements of its enum type.
     */
	template<typename E>
	constexpr auto toUType(E enumeration) noexcept
	{
		return static_cast<std::underlying_type_t<E>>(enumeration);
	}
}

#endif
