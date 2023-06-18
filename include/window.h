#ifndef OGLS_WINDOW_H
#define OGLS_WINDOW_H

#include <memory>
#include <string_view>

#include "helpers/macros.h"

namespace ogls
{
/**
 * \brief Window is a wrapper over
 * [GLFWwindow](https://www.glfw.org/docs/3.3/group__window.html#ga3c96d80d363e67d13a41b5d1821f3242).
 */
class Window
{
    private:
        /**
         * \brief Impl contains private data and methods of Window.
         */
        class Impl;

    public:
        /**
         * \brief Returns bool value: true if the window was terminated, false otherwise.
         */
        static bool isGLFWTerminated() noexcept;

        /**
         * \brief Constructs new GLFWwindow with the specified width, height and title.
         *
         * \param width  - width of the window in pixels.
         * \param height - height of the window in pixels.
         * \param title  - title of the window. Must be null-terminated C-style string.
         * \throw std::out_of_range, ogls::exceptions::WindowInitializationException().
         */
        Window(int width, int height, std::string_view title);
        Window() = delete;
        OGLS_NOT_COPYABLE_MOVABLE(Window)
        /**
         * \brief Destructs the window and OpenGL context by calling
         * [glfwTerminate()](https://www.glfw.org/docs/3.3/group__init.html#gaaae48c0a18607ea4a4ba951d939f0901).
         */
        ~Window() noexcept;

        /**
         * \brief Wraps
         * [glfwWindowShouldClose()](https://www.glfw.org/docs/3.3/group__window.html#ga24e02fbfefbb81fc45320989f8140ab5).
         *
         * \return true if should be closed, false otherwise.
         */
        bool shouldClose() const;
        /**
         * \brief Wraps
         * [glfwSwapBuffers()](https://www.glfw.org/docs/3.3/group__window.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14).
         */
        void swapBuffers();

    private:
        /**
         * \brief Pointer to implementation.
         */
        std::unique_ptr<Impl> m_impl;

};  // class Window

}  // namespace ogls

#endif
