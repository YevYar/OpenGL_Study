#include "windowHandlers.h"

void windowFramebufferSizeCalback(GLFWwindow* window, int width, int height)
{
	glfwGetFramebufferSize(window, &width, &height);
}
