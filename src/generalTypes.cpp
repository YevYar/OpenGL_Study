#include "generalTypes.h"

namespace ogls
{
ArrayData::ArrayData(const void* p, size_t s) noexcept : pointer{p}, size{s}
{
}

ArrayData::ArrayData(ArrayData&& obj) noexcept : pointer{obj.pointer}, size{obj.size}
{
    obj.pointer = nullptr;
    obj.size    = {0};
}

ArrayData& ArrayData::operator=(ArrayData&& obj) noexcept
{
    pointer = obj.pointer;
    size    = {obj.size};

    obj.pointer = nullptr;
    obj.size    = {0};

    return *this;
}

ICloneable::~ICloneable() = default;

}  // namespace ogls
