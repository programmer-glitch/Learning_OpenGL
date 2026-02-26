#include <iostream>
#include "glad/include/glad/glad.h" // API for interacting with the GPU using OpenGL specification
#include "glfw/include/GLFW/glfw3.h" // API to create an OpenGL context and window with inout support
#include <fstream>
#include <string>
#include "Shader.h"



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

	// Vertex data start
	float vertices[] = {
	// first triangle with color floats
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // EBO index 0
		-0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,// EBO index 1
		-1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,// EBO index 2

	};

	float vertices2[]{
		// second triangle
		0.5f, 0.5f, 0.0f, // EBO index 3
		-0.5f, 0.5f, 0.0f, // EBO index 1
		0.5f, -0.5f, 0.0f, // EBO index 2
	};

	// Vertex data end

	unsigned int VBO;
	// generate a buffer object name (unique identifier) in OpenGL and store it in VBO
	glGenBuffers(1, &VBO);

	unsigned int indicies[]{
		0, 1, 2,
		1, 2, 3
	};
	// Element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// Vertex array object start

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// use the buffer object as a vertex buffer object. Anytime we target GL_ARRAY_BUFFER,
	// we would refer to the recently bound buffer which in this case would be VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// allocate memory on the GPU for the buffer object currently bound to GL_ARRAY_BUFFER which in this case is VBO
	// then store the vertices with a hint on how it would be accessed in this case GL_STATIC_DRAW
	// 
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*(sizeof(float))));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	unsigned int VBO2;
	glGenBuffers(1, &VBO2);

	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 9 * sizeof(float), vertices2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	Shader shader1 = Shader("vertexShader.glsl", "fragmentShader.glsl");

	Shader shader2 = Shader("vertexShader.glsl", "fragmentShader2.glsl");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Render Loop start

	while (!(glfwWindowShouldClose(window))) {
		processInput(window);

		// rendering start

		// set the state of the display color
		glClearColor(0.25f, 0.25f, 1.0f, 1.0f);
		// replace the current state with the previously declared display state
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update state and render current state
		shader1.use();
		shader1.setFloat3("vertexOffset", 0.8f, 1.0f, 0.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);

		// ---------- Second Triangle -----------

		
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//if (uniformColorLocation == -1) {
		//	std::cout<<"Error!, could not get Uniform Location."<<std::endl;
		//	return 1;
		//}
		shader2.use();
		shader2.setFloat4("fragColor2", 0.7f, greenValue, 0.3f, 1.0f);
		shader2.setFloat3("vertexOffset", 0.0f, 0.2f, 0.0f);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
		
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

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
