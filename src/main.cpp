#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "generalTypes.h"
#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"
#include "vertexArray.h"
#include "buffer.h"
#include "vertexBufferLayout.h"
#include "shaderProgram.h"
#include "uniforms.h"
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

    // uniforms::Uniform<float, 1> U(1, "test");
    /*uniforms::Uniform<int, 2> U2("test");
    uniforms::Uniform<unsigned int, 3> U3("test");
    uniforms::Uniform<double, 4> U4("test");*/
    //uniforms::Uniform<int, 2> U5("test");
    //uniforms::Uniform<double, 5> U6("test");
    //uniforms::Uniform<double, 0> U7("test");

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

    ArrayData data{ reinterpret_cast<const void*>(points), sizeof(points)};
    vertex::Buffer VBO(vertex::BufferTarget::ARRAY_BUFFER, data, vertex::BufferDataUsage::STATIC_DRAW);

    vertex::VertexAttribute coordinateMap(0, 2, vertex::VertexAttrType::FLOAT, false, 0),
        colorMap(1, 3, vertex::VertexAttrType::FLOAT, false,
            vertex::getByteSizeOfType(vertex::VertexAttrType::FLOAT) * 3);

    vertex::VertexBufferLayout layout;
    layout.addVertexAttribute(coordinateMap);
    layout.addVertexAttribute(colorMap);

    VAO.addBuffer(VBO, layout);

    vertex::Buffer EBO(vertex::BufferTarget::ELEMENT_ARRAY_BUFFER,
        ArrayData{ reinterpret_cast<const void*>(indices), sizeof(indices) }, vertex::BufferDataUsage::STATIC_DRAW);
    EBO.bind();

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
    auto& k = shaderProgram.findUniform<float, 1>("k");

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

        k.setData(&currentK);
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
