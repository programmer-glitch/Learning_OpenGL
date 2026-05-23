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
	// cube with vertices, color and texel floats/coordinates

		 // front face
		 -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		 -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		 0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		 0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		 -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		 // top face
		 -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		 -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		 0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		 0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
		 -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,

		 // left face
		 -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		 -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		 -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		 -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,

		 // bottom face
		 -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		 // right face
		 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		 0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		 0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		 // back face
		 0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		 -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

	};
	// Vertex data and coordinates end

	// Data end

	unsigned int VBO;
	// generate a buffer object name (unique identifier) in OpenGL and store it in VBO
	glGenBuffers(1, &VBO);

	// Vertex array object recording start
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// use the buffer object as a vertex buffer object. Anytime we target GL_ARRAY_BUFFER,
	// we would refer to the recently bound buffer which in this case would be VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// allocate memory on the GPU for the buffer object currently bound to GL_ARRAY_BUFFER which in this case is VBO
	// then store the vertices with a hint on how it would be accessed in this case GL_STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, (5*36) * sizeof(float), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	// vertex position layout in vertices array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// Activate the vertex position attributes to be passed to the vertex shader program at location Zero
	glEnableVertexAttribArray(0);
	// vertex texel (or texture pixel or texture map) coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * (sizeof(float))));
	// Activate the texture map coordinates to be passed to the vertex shader program at location Two
	glEnableVertexAttribArray(1);
	// Vertex array object recording end
	
	// Create, compile and activate Shader program
	Shader shader1 = Shader("vertexShader.glsl", "fragmentShader.glsl");

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

	// prep for rendering
	shader1.use();

	//set uniform values after glUseProgram() (currently located in shader1.use()
	// set the location of the texture samplers
	shader1.setInt("texture1", 0);
	shader1.setInt("texture2", 1);

	// world position of the objects
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.5f, 0.1f, -1.0f),
		glm::vec3(1.5f, 1.5f, -2.0f),
		glm::vec3(-0.5f, 1.5f, -3.0f),
		glm::vec3(-3.0f, 0.0f, -5.0f),
		glm::vec3(-4.0f, 3.0f, -8.0f),
		glm::vec3(7.0f, -3.0f, -10.0f),
		glm::vec3(-4.5f, -3.0f, -11.0f),
		glm::vec3(-5.0f, -4.0f, -7.0f),
		glm::vec3(-1.5f, -3.0f, -6.0f),
	};

	cameraOne.MovementSpeed = 7.0f;
	cameraOne.Position = glm::vec3(0.0f, 0.0f, 7.0f);

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
		shader1.setFloat1("texMixTrans", TextureMixtransparency);
		
		// enable depth testing
		glEnable(GL_DEPTH_TEST);

		// set the state of the display color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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

		// Camera position in view space
		view = cameraOne.GetViewMatrix();

		// conversion from world space to view space via the View matrix
		glm::mat4 proj;
		proj = glm::perspective(glm::radians(cameraOne.Zoom), (800.0f / 600.0f), 0.01f, 100.0f);
		//proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.01f, 100.0f);
		// conversion from view space to clip space via the Projection matrix (no need to do this per frame)
		shader1.setMat4("Proj", 1, GL_FALSE, glm::value_ptr(proj));
		shader1.setMat4("View", 1, GL_FALSE, glm::value_ptr(view));
		
		for (int x = 0; x < 10; x++) {
			glm::mat4 model = glm::mat4(1.0f);
			// Rotate on the x-axis
			// rotation is persistent translation is not, order matters.
			model = glm::translate(model, cubePositions[x]);
			model = glm::rotate(model, (float)glfwGetTime() + (float)x, glm::vec3(1.0f, 1.0f, 0.0f));
			
			// conversion from local space to world space via the Model matrix
			shader1.setMat4("Model", 1, GL_FALSE, glm::value_ptr(model));

			// Rebind
			glBindVertexArray(VAO);

			// Render mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			// Render current state
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Render Loop end

	// Free memory
	glDeleteVertexArrays(1, &VAO);
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
		TextureMixtransparency += 0.001f;
		if (TextureMixtransparency > 0.9) {
			TextureMixtransparency = 1.0;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		TextureMixtransparency -= 0.001f;
		if (TextureMixtransparency < 0.0) {
			TextureMixtransparency = 0.0;
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
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		cameraOne.Position = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraOne.Front = glm::vec3(0.0f, 0.0f, -1.0f);
	}
}

// function definition end
