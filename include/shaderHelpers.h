#include <string>

std::string readShaderFromFile(std::string pathToShader);

GLuint createShader(GLenum shaderType, std::string shaderSource);

std::string getShaderNameByType(GLenum shaderType);

GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);
