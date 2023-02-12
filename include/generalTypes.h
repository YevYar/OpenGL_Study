#ifndef GENERAL_TYPES_H
#define GENERAL_TYPES_H

#include "helpers/macros.h"

/**
 * \brief ArrayData contains pointer to a data and
 * the size in bytes of the data.
 */
struct ArrayData
{
	public:
        /**
         * \brief Constructs new object.
         * 
         * \param pointer - pointer to the data.
         * \param size - size in bytes of the data.
         */
		ArrayData(const void* pointer, unsigned int size);

		DEFAULT_COPYABLE(ArrayData)

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
		unsigned int size = 0;

};

#endif
