#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "debugHelpers.h"
#include "generalTypes.h"
#include "helpers.h"
#include "vertexArray.h"
#include "buffer.h"
#include "vertexBufferLayout.h"
#include "shaderProgram.h"
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

    vertex::Buffer VBO(vertex::BufferTarget::ARRAY_BUFFER,
        ArrayData(points, sizeof(points)), vertex::BufferDataUsage::STATIC_DRAW);

    vertex::VertexAttribute coordinateMap(0, 2, vertex::VertexAttrType::FLOAT, false, 0),
        colorMap(1, 3, vertex::VertexAttrType::FLOAT, false,
            vertex::getByteSizeOfType(vertex::VertexAttrType::FLOAT) * 3);

    vertex::VertexBufferLayout layout;
    layout.addVertexAttribute(coordinateMap);
    layout.addVertexAttribute(colorMap);

    VAO.addBuffer(VBO, layout);

    vertex::Buffer EBO(vertex::BufferTarget::ELEMENT_ARRAY_BUFFER,
        ArrayData(indices, sizeof(indices)), vertex::BufferDataUsage::STATIC_DRAW);

    // Configure shaders
    auto vShaderSource = helpers::readStringFromFile("shaders/vs/vertexShader.vert");
    auto fShaderSource = helpers::readStringFromFile("shaders/fs/fragmentShader.frag");
    if (vShaderSource.empty() || fShaderSource.empty())
    {
        return -2;
    }

    shader::Shader vShader(shader::ShaderType::VERTEX_SHADER, vShaderSource),
        fShader(shader::ShaderType::FRAGMENT_SHADER, fShaderSource);

    if (!vShader.isValid() || !fShader.isValid())
    {
        return -3;
    }

    shader::ShaderProgram shaderProgram(vShader, fShader);
    if (!shaderProgram.isValid())
    {
        return -4;
    }
    shaderProgram.use();


    // Get location of uniform variable
    // GLCall(int kLocation = glGetUniformLocation(shaderProgram, "k"));
    // ASSERT(kLocation == -1);

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

        // GLCall(glUniform1f(kLocation, currentK));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        currentK += increment;

        window.swapBuffers();
        glfwPollEvents();
    }

    VBO.unbind();
    EBO.unbind();
    VAO.unbind();

    return 0;
}
