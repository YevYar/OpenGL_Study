#include "window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "exceptions.h"
#include "helpers/debugHelpers.h"

bool Window::s_isTerminated = false;

namespace {
    void windowFramebufferSizeCalback(GLFWwindow* window, int width, int height)
    {
        GLCall(glViewport(0, 0, width, height));
    }
}

Window::Window(int width, int height, const std::string& title)
{
    if (!glfwInit())
    {
        cleanAndThrowOnInitException("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        cleanAndThrowOnInitException("Failed to create GLFW window.");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, ::windowFramebufferSizeCalback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cleanAndThrowOnInitException("Failed to initialize GLAD.");
    }

    m_window = window;
}

Window::~Window()
{
    glfwTerminate();
    Window::s_isTerminated = true;
}

bool Window::isGLFWTerminated()
{
    return Window::s_isTerminated;
}

void Window::swapBuffers() const
{
    glfwSwapBuffers(m_window);
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::cleanAndThrowOnInitException(const std::string& errorMessage)
{
    glfwTerminate();
    Window::s_isTerminated = true;
    throw exceptions::WindowInitializationException(errorMessage);
}
