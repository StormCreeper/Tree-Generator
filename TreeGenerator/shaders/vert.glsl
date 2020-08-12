#version 330 core

layout (location = 0) in vec3 aVertexPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 pv;
uniform mat4 model;
uniform float iTime;

out float lighting;
out vec3 vertexPos;
out vec3 vertexNorm;
out vec3 ldir;

void main() {
	ldir = normalize(vec3(cos(iTime * 0.5), 1.0f, sin(iTime * 0.5)));

	vec4 world_pos = model * vec4(aVertexPos, 1.0f);
	gl_Position = pv * world_pos;

	vertexPos = world_pos.xyz;
	vertexNorm = mat3(transpose(inverse(model))) * aNormal;;
	lighting = max(dot(vertexNorm, ldir), 0.1);
}