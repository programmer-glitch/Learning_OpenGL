#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec4 fragColor1;
uniform vec3 vertexOffset;

void main(){
gl_Position = vec4(aPos.x + vertexOffset.x, aPos.y + vertexOffset.y, aPos.z + vertexOffset.z, 1.0f);
fragColor1 = vec4(aPos.x + vertexOffset.x, aPos.y + vertexOffset.y, 0.2f + vertexOffset.z, 1.0f);
}