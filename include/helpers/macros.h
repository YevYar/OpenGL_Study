#ifndef MACROS_H
#define MACROS_H

/**
 * \brief Adds default copy constructor and copy assignment operator to class.
 */
#define DEFAULT_COPYABLE(ClassX) \
	ClassX(const ClassX&) = default;\
	ClassX& operator=(const ClassX&) = default;

/**
 * \brief Adds default move constructor and move assignment operator to class.
 */
#define DEFAULT_MOVABLE(ClassX) \
	ClassX(ClassX&&) = default;\
	ClassX& operator=(ClassX&&) = default;

/**
 * \brief Adds default copy and move constructors and copy and move assignment operators to class.
 */
#define DEFAULT_COPYABLE_MOVABLE(ClassX) \
	ClassX(const ClassX&) = default;\
	ClassX(ClassX&&) = default;\
	ClassX& operator=(const ClassX&) = default;\
	ClassX& operator=(ClassX&&) = default;

/**
 * \brief Deletes copy constructor and copy assignment operator from class.
 */
#define NOT_COPYABLE(ClassX) \
	ClassX(const ClassX&) = delete;\
	ClassX& operator=(const ClassX&) = delete;

/**
 * \brief Deletes move constructor and move assignment operator from class.
 */
#define NOT_MOVABLE(ClassX) \
	ClassX(ClassX&&) = delete;\
	ClassX& operator=(ClassX&&) = delete;

/**
 * \brief Deletes copy and move constructors and copy and move assignment operators from class.
 */
#define NOT_COPYABLE_MOVABLE(ClassX) \
	ClassX(const ClassX&) = delete;\
	ClassX(ClassX&&) = delete;\
	ClassX& operator=(const ClassX&) = delete;\
	ClassX& operator=(ClassX&&) = delete;

#endif
