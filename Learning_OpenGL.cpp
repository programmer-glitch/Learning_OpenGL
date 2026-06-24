#include <iostream>
#include "glad/include/glad/glad.h" // API for interacting with the GPU using OpenGL specification.
#include "glfw/include/GLFW/glfw3.h" // API to create an OpenGL context and window with inout support
#include <fstream>
#include <string>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "Camera.h"


float TextureMixtransparency;

// deltaTime
float currentTime = 0.0f;
float lastTime = 0.0f;
float deltaTime = 0.0f;

//mouse position
float lastX = 400, lastY = 300;
float yaw = -90.0f, pitch = 0.0f;
bool firstMouse = true;

// Spawn a camera
Camera cameraOne;

// Object ambient color
float ambientValue = 0.5f;


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
	void mouseCallback(GLFWwindow* window, double xPos, double yPos);
	void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	void processInput(GLFWwindow* window);
	// function declarations end

	// set window resize callback
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	// Data Start

	// Vertex data and coordinates start
	float vertices[] = {
	// cube with vertices, texel floats/coordinates and normals for the faces which represents the corresponding vertex

		 // front face
		 -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		 // top face
		 -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		 -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

		 // left face
		 -0.5f, 0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		 -0.5f, 0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		 -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		 -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		 -0.5f, 0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

		 // bottom face
		 -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,

		 // right face
		 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

		 // back face
		 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

	};
	// Vertex data and coordinates end

	// Data end

	unsigned int VBO;
	// generate two buffer object names (unique identifier for the cube target object and light object) in OpenGL and store it in the VBO array
	glGenBuffers(1, &VBO);

	// Vertex array object recording start
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// use the buffer object as a vertex buffer object. Anytime we target GL_ARRAY_BUFFER,
	// we would refer to the recently bound buffer which in this case would be VBO
	// vertex buffer object (for the cube target object)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// allocate memory on the GPU for the buffer object currently bound to GL_ARRAY_BUFFER which in this case is VBO with a hint on how it would be accessed in this case GL_STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, (6*36) * sizeof(float), 0, GL_STATIC_DRAW);
	// Insert the vertices data(co-ordinates) into the previously allocated storage
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	// vertex position layout in vertices array
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// Activate the vertex position attributes to be passed to the vertex shader program at location Two
	glEnableVertexAttribArray(2);
	// Normals position layout in the vertices array
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// vertex texel (or texture pixel or texture map) coordinates
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * (sizeof(float))));
	//// Activate the texture map coordinates to be passed to the vertex shader program at location One
	//glEnableVertexAttribArray(1);

	// unbind VAO
	glBindVertexArray(0);

	// VBO2
	unsigned int lightVAO;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// vertex buffer object (for the cube lighting object)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	// Vertex array object recording end
	glBindVertexArray(0);
	
	// Create, compile and activate Shader program
	//Shader shader1 = Shader("lightVertexShader.glsl", "fragmentShader.glsl");
	Shader shader1 = Shader("lightVertexShader.glsl", "objectFragmentShader.glsl");
	Shader shader2 = Shader("lightVertexShader.glsl", "lightFragmentShader.glsl");

	// ------------ TEXTURES --------------

	// Texture data and coordinates start
	
	// generating a texture
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Texture Horizontal and vertical repitition on the model
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Texture mipmap interpolation for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Image width, height and number of channels
	int width, height, nrChannels;
	unsigned char* data = stbi_load("Container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		// generate a texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "failed to load image!" << std::endl;
	}
	stbi_image_free(data);

	// second texture
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Texture Horizontal and vertical repitition on the model
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Texture mipmap interpolation for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Image width, height and number of channels
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		// generate a texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "failed to load image!" << std::endl;
	}
	stbi_image_free(data);

	// Texture data and coordinates end


	// world position of the objects
	glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 lightCubePosition = glm::vec3(0.5f, 0.0f, 1.5f);

	cameraOne.MovementSpeed = 7.0f;
	cameraOne.Position = glm::vec3(0.0f, 0.0f, 7.0f);

	// prep for rendering
	shader1.use();
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
	shader1.setFloat3("lightColor", lightColor);
	//shader1.setFloat3("lightPosition", lightCubePosition);
	shader1.setFloat3("objectColor", objectColor);
	shader1.setFloat1("ambientStrength", ambientValue);
	shader1.setFloat3("viewerPos", cameraOne.Position);


	//set uniform values after glUseProgram() (currently located in shader1.use()
	// set the location of the texture samplers
	//shader1.setInt("texture1", 0);
	//shader1.setInt("texture2", 1);


	// Render Loop start
	while (!(glfwWindowShouldClose(window))) {

		// Calculate deltaTime for stable FPS
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glm::mat4 view;
		// Lookout for window input
		processInput(window);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouseCallback);
		glfwSetScrollCallback(window, scrollCallback);


		// update things
		//shader1.setFloat1("texMixTrans", TextureMixtransparency);
		
		// enable depth testing
		glEnable(GL_DEPTH_TEST);

		// set the state of the display color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// replace the current state with the previously declared display and depth state
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// update currerntly active texture
		glActiveTexture(GL_TEXTURE0);
		// bind texture sampler to currently active texture unit
		glBindTexture(GL_TEXTURE_2D, texture1);
		// update currerntly active texture
		glActiveTexture(GL_TEXTURE1);
		// bind texture sampler to currently active texture unit
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Update state
		shader1.use();

		// had to put this here so i can update the uniform regularly
		shader1.setFloat3("lightCubePosition", lightCubePosition);
		
		// Camera position in view space
		view = cameraOne.GetViewMatrix();

		// conversion from world space to view space via the View matrix
		glm::mat4 proj;
		proj = glm::perspective(glm::radians(cameraOne.Zoom), (800.0f / 600.0f), 0.01f, 100.0f);
		//proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.01f, 100.0f);
		// conversion from view space to clip space via the Projection matrix (no need to do this per frame)
		shader1.setMat4("Proj", 1, GL_FALSE, glm::value_ptr(proj));
		shader1.setMat4("View", 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePosition);
		// conversion from local space to world space via the Model matrix
		shader1.setMat4("Model", 1, GL_FALSE, glm::value_ptr(model));

		// Rebind
		glBindVertexArray(VAO);

		// Render mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Render object cube
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// unbind VAO
		glBindVertexArray(0);
		
		// switch shaders
		shader2.use();

		// Light source Cube
		glBindVertexArray(lightVAO);
		lightCubePosition = glm::vec3(0.5f * sin(glfwGetTime() * 3), 0.5f * cos(glfwGetTime() * 3), 1.5f);
		model = glm::translate(model, lightCubePosition);
		model = glm::scale(model, glm::vec3(0.25f));
		shader2.setMat4("Model", 1, GL_FALSE, glm::value_ptr(model));
		shader2.setMat4("View", 1, GL_FALSE, glm::value_ptr(view));
		shader2.setMat4("Proj", 1, GL_FALSE, glm::value_ptr(proj));
		shader2.setFloat3("lightColor", lightColor);

		// Render mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Render light source cube
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// unbind VAO
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Render Loop end

	// Free memory
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}

// function definition start

// Callback function whenever the window is resized
void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Callback function for mouse position
void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
	
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	cameraOne.ProcessMouseMovement(xOffset, yOffset);

}

// callback function for mouse zoom
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	cameraOne.ProcessMouseScroll(yOffset);
}


// Process input
void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		ambientValue += 0.001f;
		if (ambientValue > 0.9) {
			ambientValue = 1.0;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		ambientValue -= 0.001f;
		if (ambientValue < 0.0) {
			ambientValue = 0.0;
		}
	}

	// Camera control
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraOne.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraOne.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_A)) {
		cameraOne.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraOne.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}

}

// function definition end
