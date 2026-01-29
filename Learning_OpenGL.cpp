#include <iostream>
#include "glad/include/glad/glad.h" // API for interacting with the GPU using OpenGL specification
#include "glfw/include/GLFW/glfw3.h" // To create an OpenGL context and window with inout support



int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Generate window and context
	GLFWwindow* window = glfwCreateWindow(800, 600, "MyWindowTitle", NULL, NULL);
	if (window == NULL) {
		std::cout<<"Failed to create window."<<std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// initialize Glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout<<"Failed to intitialize GLAD"<<std::endl;
		return -1;
	}
	glViewport(0, 0, 800, 600);
	
	// function declaration start
	void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow * window);
	// function declaration end


	// set window resize callback
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	// Render Loop start
	while (!(glfwWindowShouldClose(window))) {
		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	} 
	// Render Loop end

	glfwTerminate();

	return 0;
}

// function definition start

// Callback function whenever the window is resized
void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Process input
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

// function definition end
