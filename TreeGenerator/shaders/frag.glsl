#version 330 core

out vec4 FragColor;

in float lighting;
in vec3 vertexPos;
in vec3 vertexNorm;
in vec3 ldir;

uniform vec3 viewPos;

void main() {
	FragColor = vec4(vec3(lighting), 1);
}