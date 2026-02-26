#version 330 core

out vec4 fragmentColor;
uniform vec4 fragColor2;

void main(){
fragmentColor = fragColor2;
}