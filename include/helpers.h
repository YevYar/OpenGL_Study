#include <string>

namespace helpers
{
	std::string readStringFromFile(std::string pathToFile);

	template<typename E>
	constexpr auto toUType(E enumeration) noexcept
	{
		return static_cast<std::underlying_type_t<E>>(enumeration);
	}
}
