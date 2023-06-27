#ifndef OGLS_HELPERS_MACROS_H
#define OGLS_HELPERS_MACROS_H

/**
 * \brief Adds default copy constructor and copy assignment operator to a class.
 *
 * \param ClassX - the name of the class.
 */
#define OGLS_DEFAULT_COPYABLE(ClassX)           \
    ClassX(const ClassX&)            = default; \
    ClassX& operator=(const ClassX&) = default;

/**
 * \brief Adds default move constructor and move assignment operator to a class.
 *
 * \param ClassX - the name of the class.
 */
#define OGLS_DEFAULT_MOVABLE(ClassX)       \
    ClassX(ClassX&&)            = default; \
    ClassX& operator=(ClassX&&) = default;

/**
 * \brief Adds default copy and move constructors and copy and move assignment operators to a class.
 *
 * \param ClassX - the name of the class.
 */
#define OGLS_DEFAULT_COPYABLE_MOVABLE(ClassX)   \
    ClassX(const ClassX&)            = default; \
    ClassX(ClassX&&)                 = default; \
    ClassX& operator=(const ClassX&) = default; \
    ClassX& operator=(ClassX&&)      = default;

/**
 * \brief Adds default copy and move constructors and copy and move assignment operators to a class.
 *
 * All added methods are constexpr and noexcept.
 *
 * \param ClassX - the name of the class.
 */
#define OGLS_DEFAULT_CONSTEXPR_NOEXCEPT_COPYABLE_MOVABLE(ClassX)   \
    constexpr ClassX(const ClassX&) noexcept            = default; \
    constexpr ClassX(ClassX&&) noexcept                 = default; \
    constexpr ClassX& operator=(const ClassX&) noexcept = default; \
    constexpr ClassX& operator=(ClassX&&) noexcept      = default;

/**
 * \brief Deletes copy constructor and copy assignment operator from a class.
 *
 * \param ClassX - the name of the class.
 */
#define OGLS_NOT_COPYABLE(ClassX)              \
    ClassX(const ClassX&)            = delete; \
    ClassX& operator=(const ClassX&) = delete;

/**
 * \brief Deletes move constructor and move assignment operator from a class.
 *
 * \param ClassX - the name of the class.
 */
#define OGLS_NOT_MOVABLE(ClassX)          \
    ClassX(ClassX&&)            = delete; \
    ClassX& operator=(ClassX&&) = delete;

/**
 * \brief Deletes copy and move constructors and copy and move assignment operators from a class.
 *
 * \param ClassX - the name of the class.
 */
#define OGLS_NOT_COPYABLE_MOVABLE(ClassX)      \
    ClassX(const ClassX&)            = delete; \
    ClassX(ClassX&&)                 = delete; \
    ClassX& operator=(const ClassX&) = delete; \
    ClassX& operator=(ClassX&&)      = delete;

#endif
