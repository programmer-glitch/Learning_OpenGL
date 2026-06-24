// OpenGL 3.30 core profile
#version 330 core

// variable values set within GLSL
in vec3 normal;
in vec3 fragPos;
out vec4 fragColor;
vec3 ambientReflection;
vec3 norm;
vec3 lightDir;
float diffDotProd;
vec3 diffuseReflection;
vec3 result;
float specularStrength = 0.5f;
vec3 viewerDir;
vec3 reflectDir;
float spec;
vec3 specularReflection;


// variable values are set outside in the cpp code file
uniform vec3 lightColor;
uniform vec3 lightCubePosition;
uniform vec3 objectColor;
uniform float ambientStrength;
uniform vec3 viewerPos;


 void main(){
	ambientReflection = lightColor * ambientStrength;
	norm = normalize(normal);
	lightDir = normalize(lightCubePosition - fragPos);
	diffDotProd = max(dot(norm, lightDir), 0.0f);
	diffuseReflection = diffDotProd * lightColor;
	viewerDir = normalize(viewerPos - fragPos);
	reflectDir = reflect(-lightDir, normal);
	spec = pow(max(dot(viewerDir, reflectDir), 0.0f), 32);
	specularReflection = specularStrength * spec * lightColor;

	result = (ambientReflection + diffuseReflection + specularReflection) * objectColor;

	fragColor = vec4(result, 1.0f);
 }