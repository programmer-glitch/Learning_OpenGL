#include <iostream>
#include "glad/include/glad/glad.h" // API for interacting with the GPU using OpenGL specification.
#include "glfw/include/GLFW/glfw3.h" // API to create an OpenGL context and window with inout support
#include <fstream>
#include <string>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



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

	// Vertex data and coordinates start
	
	float vertices[] = {
	// first rectangle with vertices, color and texel floats/coordinates
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // EBO index 0
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // EBO index 1
		 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // EBO index 2
		 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // EBO index 3 

	};

	// Vertex data and coordinates end


	// Texture data and coordinates start

	// generating a texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Texture Horizontal and vertical repitition on the model
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// Texture mipmap interpolation for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Image width, height and number of channels
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		// generate a texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		stbi_image_free(data);
	}

	// Texture data and coordinates end

	// Data end

	unsigned int VBO;
	// generate a buffer object name (unique identifier) in OpenGL and store it in VBO
	glGenBuffers(1, &VBO);

	// EBO indicies
	unsigned int indicies[]{
		0, 1, 2,
		1, 2, 3,
		
	};
	// Element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// Vertex array object to recording start
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// use the buffer object as a vertex buffer object. Anytime we target GL_ARRAY_BUFFER,
	// we would refer to the recently bound buffer which in this case would be VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// use the buffer object as a Element buffer object. Anytime we target GL_ELEMENT_ARRAY_BUFFER,
	// we would refer to the recently bound buffer which in this case would be EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// allocate memory on the GPU for the buffer object currently bound to GL_ARRAY_BUFFER which in this case is VBO
	// then store the vertices with a hint on how it would be accessed in this case GL_STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	// allocate memory on the GPU for the buffer object currently bound to GL_ELEMENT_ARRAY_BUFFER which in this case is EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	// vertex position layout in vertices array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// Activate the vertex position attributes to be passed to the vertex shader program at location Zero
	glEnableVertexAttribArray(0);
	// vertex colour layout in vertices array
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*(sizeof(float))));
	// Activate the vertex color attributes to be passed to the vertex shader program at location One
	glEnableVertexAttribArray(1);
	// vertex texel (or texture pixel or texture map) coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * (sizeof(float))));
	// Activate the texture map coordinates to be passed to the vertex shader program at location Two
	glEnableVertexAttribArray(2);

	// Vertex array object recording end
	
	// Create, compile and activate Shader program
	Shader shader1 = Shader("vertexShader.glsl", "fragmentShader.glsl");

	// Create illusion of depth based on last rendered object
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Render Loop start
	while (!(glfwWindowShouldClose(window))) {
		// Lookout for window input
		processInput(window);

		// rendering start

		// set the state of the display color
		glClearColor(0.25f, 0.25f, 1.0f, 1.0f);
		// replace the current state with the previously declared display state
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update state
		shader1.use();
		shader1.setFloat3("vertexOffset", 0.0f, 0.0f, 0.0f);
		shader1.setInt("ourTexture", 0);
		// To be doubly sure
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		// Render current state
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
		

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
