#version 330 core

in vec2 TextCoord;
out vec4 fragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float texMixTrans;

 void main(){
	fragColor = mix(texture(texture1, TextCoord), texture(texture2, TextCoord), texMixTrans);
 }