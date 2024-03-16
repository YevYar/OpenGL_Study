#include <iostream>
#include <memory>

#include <glad/glad.h>
#include <glfw3.h>

#include "exceptions.h"
#include "window.h"

#include "renderer/renderer.h"

namespace
{
constexpr auto           WIDTH  = int{800};
constexpr auto           HEIGHT = int{600};
constexpr decltype(auto) TITLE  = "OpenGL Study Project";

}  // namespace

int main()
{
    using namespace ogls;
    using namespace ogls::exceptions;
    using namespace ogls::renderer;


    auto window = std::unique_ptr<Window>{};
    try
    {
        window = std::make_unique<Window>(WIDTH, HEIGHT, TITLE);
    }
    catch (const WindowInitializationException& exc)
    {
        std::cerr << exc.what() << std::endl;
        return -1;
    }

    glfwSwapInterval(4);

    auto renderer = std::unique_ptr<Renderer>{};
    try
    {
        renderer = std::make_unique<Renderer>();
    }
    catch (const GLRecAcquisitionException& exc)
    {
        std::cerr << exc.what() << std::endl;
        return -2;
    }

    // Render loop
    while (!window->shouldClose())
    {
        renderer->render();

        window->swapBuffers();

        glfwPollEvents();
    }

    return 0;
}
