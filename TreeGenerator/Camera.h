#pragma once
#include "utils.h"

typedef class Camera Camera;
class Camera {
private:
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	glm::vec2 rotation;
	float speed;
	float lastX;
	float lastY;
	bool firstMouse;
public:

	Camera(float aspect, glm::vec3 pos);

	void setUniforms(unsigned int shader, bool reset);
	void updateModel(unsigned int shader);
	void translate(glm::vec3 vector);
	void rotate(float angle, glm::vec3 axis);
	void identity();
	void updateInput(GLFWwindow* window, float deltaTime);
	void mouse_callback(GLFWwindow* window, float posx, float posy);

	glm::vec3 getPos();

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 front;
};