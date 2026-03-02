#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexture;

out vec4 fragColor1;
uniform vec3 vertexOffset;
out vec2 TextCoord;

void main(){
gl_Position = vec4(aPos.x + vertexOffset.x, aPos.y + vertexOffset.y, aPos.z + vertexOffset.z, 1.0f);
fragColor1 = vec4(aColor, 1.0f);
TextCoord = aTexture;
}