// OpenGL version 3.30 core profile
#version 330 core
layout (location = 2) in vec3 bPos;
layout (location = 1) in vec3 bNorm;

out vec3 normal;
out vec3 fragPos;

vec3 norm;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

void main(){ 
	gl_Position = Proj * View * Model * vec4(bPos, 1.0f);
	fragPos = vec3(Model * (vec4(bPos, 1.0f)));
	norm = mat3(transpose(inverse(Model))) * bNorm;
	normal = norm;
}