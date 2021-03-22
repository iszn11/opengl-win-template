#include "glad.h"
#include <GLFW/glfw3.h>

#include "game.h"

constexpr int DEFAULT_WINDOW_WIDTH = 720;
constexpr int DEFAULT_WINDOW_HEIGHT = 480;
constexpr const char* WINDOW_TITLE = "OpenGL Template";

static void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);

int __stdcall wWinMain(void* hInstance, void* hPrevInstance, wchar_t* pCmdLine, int nCmdShow)
{
	if (!glfwInit())
	{
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return 1;
	}

	glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		return 1;
	}

	init();

	double t0 = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		double t1 = glfwGetTime();
		double dt = t1 - t0;
		t0 = t1;

		update((float)dt);
		draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	deinit();

	return 0;
}

static void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
