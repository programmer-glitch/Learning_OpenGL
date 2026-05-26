#version 330 core
layout (location = 2) in vec3 bPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

void main(){ 
gl_Position = Proj * View * Model * vec4(bPos, 1.0f);
}