#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "debugHelpers.h"
#include "shaderHelpers.h"
#include "windowHandlers.h"

int main()
{
    GLFWwindow* window = init();

    if (!window) {
        return -1;
    }

    const float points[] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    // Configure VAO and VBO
    unsigned int VAO;
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glBindVertexArray(VAO));

    unsigned int VBO;
    GLCall(glGenBuffers(1, &VBO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, reinterpret_cast<void*>(0)));
    GLCall(glEnableVertexAttribArray(0));

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


    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        GLCall(glClearColor(0.1176f, 0.5647, 1.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glDeleteBuffers(1, &VBO));

    GLCall(glBindVertexArray(0));
    GLCall(glDeleteVertexArrays(1, &VAO));

    GLCall(glDeleteProgram(shaderProgram));

    glfwTerminate();
    
    return 0;
}
