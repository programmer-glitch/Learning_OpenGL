// OpenGL 3.30 with core profile
#version 330 core

// GLSL variable set within the file
out vec4 fragColor;

// Uniform variable set outside the file
uniform vec3 lightColor;


 void main(){
	fragColor = vec4(lightColor, 1.0f);
 }