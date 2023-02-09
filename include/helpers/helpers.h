#ifndef HELPERS_H
#define HELPERS_H

#include <string>

namespace helpers
{
    /**
     * \brief Opens the file and reads the content.
     * 
     * \param pathToFile is a path to file to be read
     * \throw exceptions::FileOpeningException(), exceptions::FileReadingException()
     */
	std::string readStringFromFile(const std::string& pathToFile);

    /**
     * \brief Returns a type of elements of a passed enum type.
     */
	template<typename E>
	constexpr auto toUType(E enumeration) noexcept
	{
		return static_cast<std::underlying_type_t<E>>(enumeration);
	}
}

#endif
