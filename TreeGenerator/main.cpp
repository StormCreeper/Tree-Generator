#include "utils.h"
#include "Scene.h"
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double posx, double posy);

int main() {
	Scene::init(1200, 700);
	 
	float lastTime = 0;
	float count = 0;
	float medDelta = 0;
	float deltaTime = 0;
	float currentTime;
	char title[] = "Tree Generator - 0000 FPS";

	glfwSetCursorPosCallback(Scene::getWindow(), (GLFWcursorposfun)mouse_callback);
	glfwSetMouseButtonCallback(Scene::getWindow(), (GLFWmousebuttonfun)Scene::mouse_button_callback);
	glfwSetInputMode(Scene::getWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	//glfwSetWindowShouldClose(Scene::getWindow(), true);

	while (!glfwWindowShouldClose(Scene::getWindow())) {
		currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastTime;
		//if (deltaTime < 1.0f / 60.0f) continue;
		medDelta += deltaTime;
		if (medDelta < 1) count++;
		else {
			sprintf_s(title, 28, "Tree Generator - %d FPS", (int)(1.0f / medDelta * (count+1)) + 1);
			glfwSetWindowTitle(Scene::getWindow(), title);
			count = 0;
			medDelta = 0;
		}
		lastTime = currentTime;
		
		processInput(Scene::getWindow());
		Scene::renderFrame(currentTime, deltaTime);
	}

	Scene::deleteScene();
	return 0;
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow* window, double posx, double posy) {
	Scene::getCamera()->mouse_callback(window, (float)posx, (float)posy);
}