#include <iostream>
#include "glad/include/glad/glad.h" // API for interacting with the GPU using OpenGL specification
#include "glfw/include/GLFW/glfw3.h" // API to create an OpenGL context and window with inout support
#include <fstream>
#include <string>



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
	// first triangle
		-0.5f, -0.5f, 0.0f, // EBO index 0
		-0.5f, 0.5f, 0.0f, // EBO index 1
		0.5f, -0.5f, 0.0f, // EBO index 2

	// second triangle
		0.5f, 0.5f, 0.0f, // EBO index 3
		0.5f, -0.5f, 0.0f,
		1.0f, -0.5f, 0.0f,
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// vertex array object end

	// Data End



	// GLSL program start

	// Vertex shader start

	std::string vertexShaderStr;
	std::ifstream vertexShaderFile("vertexShader.glsl");
	if (!(vertexShaderFile.is_open())) {
		std::cout<< "Error! could not open Vertex Shader file!\n";
		return 1;
	}
	std::string buffer;
	while (std::getline(vertexShaderFile, buffer)) {
		vertexShaderStr += buffer + '\n';
	}
	vertexShaderFile.close();

	const char* vertexShaderSource = vertexShaderStr.c_str();
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &(vertexShaderSource), NULL);
	glCompileShader(vertexShader);
	int compileStatus;
	char shaderLog[1000];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus) {
		std::cout<<"vertex shader compilation successful!\n";
	}
	else {
		glGetShaderInfoLog(vertexShader, sizeof(shaderLog), NULL, shaderLog);
		std::cout<<"Error!, vertex shader failed to compile\n"<<shaderLog<<std::endl;
		return 1;
	}

	// Vertex shader end

	// Fragment shader start

	std::string fragmentShaderStr;
	std::ifstream fragmentShaderFile("fragmentShader.glsl");
	if (!fragmentShaderFile.is_open()) {
		std::cout<<"Error! Could not open Fragment Shader File!\n";
		return 1;
	}
	while (std::getline(fragmentShaderFile, buffer)) {
		fragmentShaderStr += buffer + '\n';
	}
	fragmentShaderFile.close();

	const char* fragmentShaderSource = fragmentShaderStr.c_str();
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus) {
		std::cout<<"Fragment Shader compilation successful!\n";
	}
	else {
		glGetShaderInfoLog(fragmentShader, sizeof(shaderLog), NULL, shaderLog);
		std::cout<<"Error!, fragment shader failed to compile\n"<<shaderLog<<std::endl;
		return 1;
	}

	// Fragment shader end

	// Shader Program (for rendering) start

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	int linkingStatus;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkingStatus);
	if (linkingStatus) {
		std::cout<<"Shader Program object linking successful!\n";
	}
	else {
		glGetProgramInfoLog(shaderProgram, sizeof(shaderLog), NULL, shaderLog);
		std::cout<<"Error, Shader program linking failed\n"<<shaderLog<<std::endl;
	}
	glUseProgram(shaderProgram);

	// Shader Program end

	// GLSL program end



	// Vertex data format fed to GPU start

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Vertex data format fed to GPU end
	


	

	// Render Loop start

	while (!(glfwWindowShouldClose(window))) {
		processInput(window);

		// rendering start

		// set the state of the display color
		glClearColor(0.25f, 0.25f, 1.0f, 1.0f);
		// replace the current state with the previously declared display state
		glClear(GL_COLOR_BUFFER_BIT);


		// Update state and render current state
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
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
