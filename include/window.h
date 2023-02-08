#ifndef WINDOW_H
#define WINDOW_H

#include <string>

class GLFWwindow;

class Window
{
	public:
		Window(int width, int height, std::string title);
		~Window();

		static bool isGLFWTerminated();

		void swapBuffers() const;
		bool shouldClose() const;
		bool isInitialized() const;

	private:
		GLFWwindow* m_window = nullptr;

		static bool isTerminated;

};

#endif
