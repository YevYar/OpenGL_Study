#include "window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "helpers/debugHelpers.h"

bool Window::isTerminated = false;

void windowFramebufferSizeCalback(GLFWwindow* window, int width, int height)
{
    GLCall(glViewport(0, 0, width, height));
}

Window::Window(int width, int height, std::string title)
{
    // TODO: throw error, no return
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
        Window::isTerminated = true;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        Window::isTerminated = true;
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, windowFramebufferSizeCalback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        Window::isTerminated = true;
        return;
    }

    m_window = window;
}

Window::~Window()
{
    glfwTerminate();
    Window::isTerminated = true;
}

bool Window::isGLFWTerminated()
{
    return Window::isTerminated;
}

void Window::swapBuffers() const
{
    glfwSwapBuffers(m_window);
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

bool Window::isInitialized() const
{
    return m_window != nullptr;
}
