#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    // Configure shaders
    auto vShaderSource = readShaderFromFile("shaders/vs/vertexShader.shader");
    auto fShaderSource = readShaderFromFile("shaders/fs/fragmentShader.shader");
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
    glUseProgram(shaderProgram);


    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1176f, 0.5647, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBO);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);

    glDeleteProgram(shaderProgram);

    glfwTerminate();
    
    return 0;
}
