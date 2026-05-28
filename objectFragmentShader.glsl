#version 330 core

out vec4 fragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;
vec3 finalColor;

 void main(){
	finalColor = lightColor * objectColor;
	fragColor = vec4(finalColor, 1.0f);
 }