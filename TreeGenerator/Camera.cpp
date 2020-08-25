#include "Camera.h"

Camera::Camera(float aspect, glm::vec3 pos) : lastX(0), lastY(0), speed(7.0f) {
	projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians<float>(80), aspect, 0.01f, 1200.0f);
	view = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	position = pos;
	velocity = glm::vec3(0, 0, 0);
	rotation = glm::vec2(0, 0);
	
	front = glm::vec3(0, 0, 1);
	up = glm::vec3(0, 1, 0);
	worldUp = glm::vec3(0, 1, 0);
	right = glm::vec3(1, 0, 0);

	firstMouse = true;
}

void Camera::identity() {
	//view = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
}

void Camera::setUniforms(unsigned int shader, bool reset) {
	if (reset) {
		identity();
		view = glm::lookAt(position, position + front, up);
	}
} 
void Camera::updateModel(unsigned int shader) {
	setUniformM4(shader, "pv", projection * view);
	setUniformM4(shader, "model", model);
	setUniformVec3(shader, "viewPos", position);
}
void Camera::translate(glm::vec3 vector) {
	model = glm::translate(model, vector);
}
void Camera::rotate(float angle, glm::vec3 axis) {
	model = glm::rotate(model, angle, axis);
}

void Camera::updateInput(GLFWwindow* window, float deltaTime) {
	float acc = 1;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) acc *= 8.0f;
	glm::vec3 tmpFront = glm::normalize(glm::vec3(front.x, 0, front.z));
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) velocity += acc * speed * tmpFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) velocity -= acc * speed * tmpFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) velocity -= acc * speed * right;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) velocity += acc * speed * right;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) velocity.y = 10;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) position -= acc * speed * deltaTime * worldUp;
	
	position += velocity * deltaTime;
	velocity.x = 0;
	velocity.y = 0;
	velocity.z = 0;
}
void Camera::mouse_callback(GLFWwindow* window, float posx, float posy) {
	if (firstMouse) {
		lastX = posx;
		lastY = posy;
		firstMouse = false;
	}

	float xoffset = posx - lastX;
	float yoffset = lastY - posy;
	lastX = 400;
	lastY = 300;

	float sensitivity = 0.2f;

	rotation.y += xoffset * sensitivity;
	rotation.x += yoffset * sensitivity;

	if (rotation.x > 89.0f)
		rotation.x = 89.0f;
	if (rotation.x < -89.0f)
		rotation.x = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	direction.y = sin(glm::radians(rotation.x));
	direction.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	front = glm::normalize(direction);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

	glfwSetCursorPos(window, 400, 300);
}

glm::vec3 Camera::getPos() {
	return position;
}