#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

unsigned int createShader(unsigned int shaderType, const char* shaderSource); 
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
unsigned int createProgram(const char* vname, const char* fname);
char* readSource(const char* filename);
void setUniformM4(unsigned int shader, const char* name, glm::mat4 matrix);
void setUniformVec3(unsigned int shader, const char* name, glm::vec3 vector);
void setUniformInt(unsigned int shader, const char* name, int value);
void setUniformFloat(unsigned int shader, const char* name, float value);

float lerp(float a, float b, float c);