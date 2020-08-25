#version 330 core

out vec4 FragColor;

in float lighting;
in vec3 vertexPos;
in vec3 vertexNorm;
in vec3 ldir;
in vec3 color;

uniform vec3 viewPos;

void main() {
	FragColor = vec4(lighting * color, 1);
}