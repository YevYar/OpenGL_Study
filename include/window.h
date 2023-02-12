#ifndef WINDOW_H
#define WINDOW_H

#include <string>

class GLFWwindow;

/**
 * \brief Window is a wrapper over 
 * [GLFWwindow](https://www.glfw.org/docs/3.3/group__window.html#ga3c96d80d363e67d13a41b5d1821f3242).
 */
class Window
{
	public:
        /**
         * \brief Constructs new GLFWwindow with the specified width, height and title.
         * 
         * \param width - width of the window in pixels.
         * \param height - height of the window in pixels.
         * \param title - title of the window.
         * \throw exceptions::WindowInitializationException().
         */
		Window(int width, int height, const std::string& title);

        /**
         * \brief Destructs the window and OpenGL context by calling
         * [glfwTerminate()](https://www.glfw.org/docs/3.3/group__init.html#gaaae48c0a18607ea4a4ba951d939f0901).
         */
		~Window();

        /**
         * \brief Returns bool value: true is the window was terminated, false otherwise.
         */
		static bool isGLFWTerminated();

        /**
         * \brief Wraps 
         * [glfwSwapBuffers()](https://www.glfw.org/docs/3.3/group__window.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14).
         */
		void swapBuffers() const;

        /**
         * \brief Wraps 
         * [glfwWindowShouldClose()](https://www.glfw.org/docs/3.3/group__window.html#ga24e02fbfefbb81fc45320989f8140ab5).
         * 
         * \return true if should be closed, false otherwise.
         */
		bool shouldClose() const;

    private:
        void cleanAndThrowOnInitException(const std::string& errorMessage);

	private:
        /**
         * \brief Pointer to controlled GLFWwindow object.
         */
		GLFWwindow* m_window = nullptr;

		static bool isTerminated;

};

#endif
