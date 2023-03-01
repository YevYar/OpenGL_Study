#include "generalTypes.h"

ArrayData::ArrayData(const void* p, unsigned int s) : pointer{ p }, size{ s }
{
}

ArrayData::ArrayData(ArrayData&& obj) noexcept : pointer{ obj.pointer }, size{ obj.size }
{
	obj.pointer = nullptr;
	obj.size = 0;
}

ArrayData& ArrayData::operator=(ArrayData&& obj) noexcept
{
	pointer = obj.pointer;
	size = obj.size;

	obj.pointer = nullptr;
	obj.size = 0;

	return *this;
}
