#ifndef OGLS_OGLCORE_OPENGL_LIMITS_H
#define OGLS_OGLCORE_OPENGL_LIMITS_H

#include <map>

#include <glad/glad.h>

namespace ogls::oglCore
{
/**
 * \brief LimitName specifies some necessary limit parameters, which are used in the program.
 *
 * See [glGet()](https://docs.gl/gl4/glGet).
 */
enum class LimitName : GLenum
{
    MaxCombinedTextureImageUnits = 0x8B'4D,
    MaxVertexAttribs             = 0x88'69
};

/**
 * \brief Returns the value of the specified limit.
 *
 * If initOpenglLimits() hasn't been called before, it throws std::logic_error.
 *
 * \param limitName - a limit parameter, the value of which is needed.
 * \return the value of the limit.
 * \throw std::logic_error.
 */
GLint getOpenglLimit(LimitName limitName);

/**
 * \brief Returns the map with values of all limits.
 *
 * If initOpenglLimits() hasn't been called before, the map is empty.
 *
 * \return the map with values of all limits.
 */
const std::map<LimitName, GLint>& getOpenglLimits() noexcept;

/**
 * \brief Retrieves values of all limits from LimitName from OpenGL state machine.
 *
 * This function must be called to allow correct usage of getOpenglLimit(), getOpenglLimits() etc.
 */
void initOpenglLimits();

}  // namespace ogls::oglCore

#endif
