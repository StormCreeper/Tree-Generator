#pragma once
#include "utils.h"
#include "Camera.h"
#include "Tree.h"

class Scene {
private:
	static int width, height;
	static GLFWwindow* window;
	static Camera* camera;

	static unsigned int shader;

	static Tree tree;

public:
	static void init(int width, int height);
	static void initializeOpenGL();
	static void createShaders();

	static void deleteScene();

	static void renderFrame(float time, float deltaTime);

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	static GLFWwindow* getWindow();
	static Camera* getCamera();
	static float getAspectRatio();
};
