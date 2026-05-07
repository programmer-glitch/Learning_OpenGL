#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;

out vec4 fragColor1;
uniform mat4 Offset;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;
out vec2 TextCoord;

void main(){
gl_Position = Proj * View * Model * vec4(aPos, 1.0f);
TextCoord = aTexture;
}