#ifndef GENERAL_TYPES_H
#define GENERAL_TYPES_H

#include "helpers/macros.h"

struct ArrayData
{
	public:
		ArrayData(const void* pointer, unsigned int size);

		DEFAULT_COPYABLE(ArrayData)

		ArrayData(ArrayData&& obj) noexcept;
		ArrayData& operator=(ArrayData&& obj) noexcept;

	public:
		const void* pointer = nullptr;
		unsigned int size = 0;

};

#endif
