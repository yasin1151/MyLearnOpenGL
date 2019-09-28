#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// #include "Test/ShaderTest.hpp"
#include "Test/TextureTest.hpp"


using namespace  std;


void OnFrameBufferSizeChange(GLFWwindow* pWindow, int iWidth, int iHeight);
void ProcessInput(GLFWwindow* pWindow);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* szTitle = "MyTestWindow";


void TestLoop(GLFWwindow* pWindow)
{
	// ShaderTestLoop(pWindow);
	TextureTestTestLoop(pWindow);
}

int main()
{
	cout << "Demo Start!!!" << endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* pWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, szTitle, nullptr, nullptr);
	if (!pWindow)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(pWindow);
	glfwSetFramebufferSizeCallback(pWindow, OnFrameBufferSizeChange);

	// load glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to Init GLAD" << endl;
		glfwTerminate();
		return -1;
	}

	TestLoop(pWindow);

	glfwTerminate();
	return 0;
}

void ProcessInput(GLFWwindow* pWindow)
{
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(pWindow, true);
	}
}

void OnFrameBufferSizeChange(GLFWwindow* pWindow, int iWidth, int iHeight)
{
	glViewport(0, 0, iWidth, iHeight);
}