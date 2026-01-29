#include <iostream>
#include "glad/include/glad/glad.h" // API for interacting with the GPU using OpenGL specification
#include "glfw/include/GLFW/glfw3.h" // To create an OpenGL context and window with inout support



int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Generate window and OpenGL context
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
	
	// function declarations start

	void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow * window);

	// function declarations end

	// set window resize callback
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	// Data Start

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

	unsigned int VBO;
	// generate a buffer object name in OpenGL and assign one unique integer to the VBO variable
	glGenBuffers(1, &VBO);

	// use the buffer object as a vertex buffer object. Anytime we target GL_ARRAY_BUFFER,
	// we would refer to the recently bound buffer which in this case would be VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// allocate memory on the GPU for the buffer object currently bound to GL_ARRAY_BUFFER which in this case is VBO
	// then store the vertices with a hint on how it would be accessed in this case GL_STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Data End

	// Render Loop start

	while (!(glfwWindowShouldClose(window))) {
		processInput(window);

		// rendering start

		// set the state of the display color
		glClearColor(0.25f, 0.25f, 1.0f, 1.0f);
		// replace the current state with the previously declared display state
		glClear(GL_COLOR_BUFFER_BIT);

		// rendering end

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
