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

	float vertices2[]{
		// second triangle
		-0.7f, -0.7f, 0.0f,
		 0.0f, 0.7f, 0.0f, 
		 0.7f, -0.7f, 0.0f,
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
	
	//Texture border color (if needed)
	/*float textureBorderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, textureBorderColor);*/

	// Texture pixel (texel) interpolation for both minification and magnification
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

	unsigned int indicies[]{
		0, 1, 2,
		1, 2, 3,
		
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// allocate memory on the GPU for the buffer object currently bound to GL_ARRAY_BUFFER which in this case is VBO
	// then store the vertices with a hint on how it would be accessed in this case GL_STATIC_DRAW
	// 
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	
	// vertex position layout in vertices array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// vertex colour layout in vertices array
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*(sizeof(float))));
	glEnableVertexAttribArray(1);

	// vertex texel (or texture map) coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * (sizeof(float))));
	glEnableVertexAttribArray(2);

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
		shader1.setFloat3("vertexOffset", 0.0f, 0.0f, 0.0f);
		shader1.setInt("ourTexture", 0);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);


		// ---------- Second Triangle -----------

		
		//float timeValue = glfwGetTime();
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//if (uniformColorLocation == -1) {
		//	std::cout<<"Error!, could not get Uniform Location."<<std::endl;
		//	return 1;
		//}
		//shader2.use();
		//shader2.setFloat4("fragColor2", 0.7f, greenValue, 0.3f, 1.0f);
		//shader2.setFloat3("vertexOffset", 0.0f, 0.05f, 0.0f);
		//glBindVertexArray(VAO2);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
		

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
