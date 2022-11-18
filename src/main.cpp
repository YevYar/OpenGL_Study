#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "debugHelpers.h"
#include "shaderHelpers.h"
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"
#include "vertexHelpers.h"
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
    vertex::VertexArray VAO;

    vertex::VertexBuffer VBO(vertex::VertexData(points, sizeof(points)), vertex::DataUsage::STATIC_DRAW);

    vertex::VertexAttribute coordinateMap(0, 2, vertex::VertexAttrType::FLOAT, false, 0),
        colorMap(1, 3, vertex::VertexAttrType::FLOAT, false,
            vertex::getByteSizeOfType(vertex::VertexAttrType::FLOAT) * 3);

    vertex::VertexBufferLayout layout;
    layout.addVertexAttribute(coordinateMap);
    layout.addVertexAttribute(colorMap);

    VAO.addBuffer(VBO, layout);

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

    VBO.unbind();

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glDeleteBuffers(1, &EBO));

    VAO.unbind();

    GLCall(glDeleteProgram(shaderProgram));

    return 0;
}
