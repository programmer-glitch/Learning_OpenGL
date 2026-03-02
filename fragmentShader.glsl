#version 330 core

in vec4 fragColor1;
in vec2 TextCoord;
out vec4 fragColor;

uniform sampler2D ourTexture;

 void main(){
	fragColor = texture(ourTexture, TextCoord) * fragColor1;
 }