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

	void frame_buffer_size_callback(GLFWwindow* window, int width, int height);

	// set window resize callback
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	// Render Loop
	while (!(glfwWindowShouldClose(window))) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	} 
	// End of Render Loop

	glfwTerminate();

	return 0;
}

// Callback function whenever the window is resized
void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}