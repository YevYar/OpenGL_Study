#include "window.h"

#include <stdexcept>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "exceptions.h"
#include "helpers/debugHelpers.h"

namespace ogls
{
namespace
{
    void windowFramebufferSizeCalback(GLFWwindow* window, int width, int height);


    auto isTerminated = false;

}  // namespace

class Window::Impl
{
    public:
        Impl(int width, int height, std::string_view title)
        {
            if (width <= 0 || height <= 0)
            {
                throw std::out_of_range{"Width and height must be more than 0."};
            }

            if (!glfwInit())
            {
                cleanAndThrowOnInitException("Failed to initialize GLFW.");
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            auto tempWindow = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
            if (!tempWindow)
            {
                cleanAndThrowOnInitException("Failed to create GLFW window.");
            }

            glfwMakeContextCurrent(tempWindow);
            glfwSetFramebufferSizeCallback(tempWindow, windowFramebufferSizeCalback);

            if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
            {
                cleanAndThrowOnInitException("Failed to initialize GLAD.");
            }

            window = tempWindow;
        }

        Impl() = delete;

        void cleanAndThrowOnInitException(std::string_view errorMessage)
        {
            glfwTerminate();
            isTerminated = true;
            throw exceptions::WindowInitializationException{errorMessage};
        };

    public:
        /**
         * \brief Pointer to controlled GLFWwindow object.
         */
        GLFWwindow* window = nullptr;

};  // class Window::Impl

Window::Window(int width, int height, std::string_view title) :
    m_impl{std::make_unique<Impl>(width, height, std::move(title))}
{
}

Window::~Window() noexcept
{
    try
    {
        glfwTerminate();
    }
    catch (...)
    {
    }

    isTerminated = true;
}

bool Window::isGLFWTerminated() noexcept
{
    return isTerminated;
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_impl->window);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(m_impl->window);
}

namespace
{
    void windowFramebufferSizeCalback(GLFWwindow* window, int width, int height)
    {
        OGLS_GLCall(glViewport(0, 0, width, height));
    }

}  // namespace

}  // namespace ogls
