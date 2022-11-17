#ifndef WINDOW_H
#define WINDOW_H

#include <string>

class GLFWwindow;

class Window
{
	public:
		Window(int width, int height, std::string title);
		~Window();

		void swapBuffers() const;
		bool shouldClose() const;
		bool isInitialized() const;

	private:
		GLFWwindow* m_window = nullptr;

};

#endif
