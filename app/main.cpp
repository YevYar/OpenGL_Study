#include <glad/glad.h>
// #include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

#include "exceptions.h"
#include "helpers/debugHelpers.h"
#include "multicoloredRectangle.h"
#include "openglLimits.h"
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

    initOpenglLimits();

    std::unique_ptr<renderer::MulticoloredRectangle> mColoredRectangle = nullptr;
    try
    {
        mColoredRectangle = renderer::makeMulticoloredRectangle();
    }
    catch (std::exception& err)
    {
        std::cerr << err.what() << std::endl;
        return -2;
    }

    float currentK = 0.0f;
    float increment = 0.05f;

    // Render loop
    while (!window->shouldClose())
    {
        GLCall(glClearColor(0.1176f, 0.5647, 1.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        if (currentK >= 1.0f)
            increment = -0.05f;
        if (currentK <= 0)
            increment = 0.05f;

        mColoredRectangle->setColorCoefficient(&currentK);
        mColoredRectangle->draw();
        
        currentK += increment;

        window->swapBuffers();
        // glfwPollEvents();
    }

    /*VBO->unbind();
    EBO->unbind();
    VAO.unbind();*/

    return 0;
}
