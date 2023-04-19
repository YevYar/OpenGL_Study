#ifndef OGLS_GENERAL_TYPES_H
#define OGLS_GENERAL_TYPES_H

#include "helpers/macros.h"

/**
 * \brief ogls namespace contains all stuff of the library OpenGL_Study.
 */
namespace ogls
{
/**
 * \brief ArrayData contains pointer to a data and the size in bytes of the data.
 */
struct ArrayData
{
    public:
        OGLS_DEFAULT_COPYABLE(ArrayData)
        /**
         * \brief Constructs new object.
         *
         * ArrayData doesn't take an ownership of the data.
         *
         * \param pointer - pointer to the data.
         * \param size    - size in bytes of the data.
         */
        ArrayData(const void* pointer, size_t size);
        /**
         * \brief Constructs new ArrayData as move-copy of other ArrayData.
         *
         * The state of other ArrayData object is set to 'no-data' state.
         */
        ArrayData(ArrayData&& obj) noexcept;

        /**
         * \brief Move-copies the state of other ArrayData.
         *
         * The state of other ArrayData object is set to 'no-data' state.
         */
        ArrayData& operator=(ArrayData&& obj) noexcept;

    public:
        /**
         * \brief Pointer to the data.
         */
        const void* pointer = nullptr;
        /**
         * \brief Size in bytes of the data.
         */
        size_t      size    = {0};

};  // struct ArrayData

/**
 * \brief ICloneable is an interface class, which provides the interface of cloneable objects.
 */
class ICloneable
{
    public:
        virtual ~ICloneable() = 0;

        /**
         * \brief Creates a deep copy of yourself.
         *
         * \return deep copy of yourself.
         */
        virtual ICloneable* clone() const = 0;

};  // class ICloneable

}  // namespace ogls

#endif
