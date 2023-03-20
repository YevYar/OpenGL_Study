#include <glad/glad.h>
// #include <GLFW/glfw3.h>

#include <memory>
#include <iostream>
#include "exceptions.h"
#include "helpers/debugHelpers.h"

#include "renderer.h"
#include "window.h"

namespace
{
    constexpr int WIDTH = 800, HEIGHT = 600;
    constexpr char TITLE[] = "OpenGL Study Project";
}

int main()
{
    std::unique_ptr<Window> window;
    try
    {
        window = std::make_unique<Window>(WIDTH, HEIGHT, TITLE);
    }
    catch (exceptions::WindowInitializationException& exc)
    {
        std::cerr << exc.what() << std::endl;
        return -1;
    }
    
    // glfwSwapInterval(4);
    std::unique_ptr<app::renderer::Renderer> renderer = nullptr;
    try
    {
        renderer = std::make_unique<app::renderer::Renderer>();
    }
    catch (exceptions::GLRecAcquisitionException& exc)
    {
        std::cerr << exc.what() << std::endl;
        return -2;
    }
    
    // Render loop
    while (!window->shouldClose())
    {
        renderer->draw();        

        window->swapBuffers();
        // glfwPollEvents();
    }

    /*VBO->unbind();
    EBO->unbind();
    VAO.unbind();*/

    return 0;
}
