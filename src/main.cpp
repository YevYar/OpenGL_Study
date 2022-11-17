#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "debugHelpers.h"
#include "shaderHelpers.h"
#include "vertexBuffer.h"
#include "window.h"

static constexpr int WIDTH = 800, HEIGHT = 600;
static constexpr char TITLE[] = "OpenGL Study Project";

int main()
{
    Window window(WIDTH, HEIGHT, TITLE);
    if (!window.isInitialized()) {
        return -1;
    }

    glfwSwapInterval(4);

    const float points[] = {
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f
    };
    const float points2[] = {
        -1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 0.0f, 1.0f
    };
    const unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Configure VAO, VBO and EBO
    unsigned int VAO;
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glBindVertexArray(VAO));

    vertex::VertexBuffer vbo(vertex::VertexData(points, sizeof(points)), vertex::DataUsage::STATIC_DRAW);

    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(0)));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 3)));
    GLCall(glEnableVertexAttribArray(1));

    unsigned int EBO;
    GLCall(glGenBuffers(1, &EBO));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));


    // Configure shaders
    auto vShaderSource = readShaderFromFile("shaders/vs/vertexShader.vert");
    auto fShaderSource = readShaderFromFile("shaders/fs/fragmentShader.frag");
    if (vShaderSource.empty() || fShaderSource.empty())
    {
        glfwTerminate();
        return -2;
    }

    auto vShader = createShader(GL_VERTEX_SHADER, vShaderSource);
    auto fShader = createShader(GL_FRAGMENT_SHADER, fShaderSource);
    if (vShader == 0 || fShader == 0)
    {
        glfwTerminate();
        return -3;
    }

    auto shaderProgram = createShaderProgram(vShader, fShader);
    if (shaderProgram == 0)
    {
        glfwTerminate();
        return -4;
    }
    GLCall(glUseProgram(shaderProgram));


    // Get location of uniform variable
    GLCall(int kLocation = glGetUniformLocation(shaderProgram, "k"));
    ASSERT(kLocation == -1);

    float currentK = 0.0f;
    float increment = 0.05f;

    // Render loop
    while (!window.shouldClose())
    {
        GLCall(glClearColor(0.1176f, 0.5647, 1.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        if (currentK >= 1.0f)
            increment = -0.05f;
        if (currentK <= 0)
            increment = 0.05f;

        GLCall(glUniform1f(kLocation, currentK));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        currentK += increment;

        window.swapBuffers();
        glfwPollEvents();
    }

    vbo.unbind();

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glDeleteBuffers(1, &EBO));

    GLCall(glBindVertexArray(0));
    GLCall(glDeleteVertexArrays(1, &VAO));

    GLCall(glDeleteProgram(shaderProgram));

    return 0;
}
