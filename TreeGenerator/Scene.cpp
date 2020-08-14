#include "Scene.h"
#include <time.h>

#define NUMTREES 5

int Scene::width(800);
int Scene::height(600);
GLFWwindow* Scene::window(NULL);
Camera* Scene::camera(NULL);

unsigned int Scene::shader;
Tree Scene::tree[NUMTREES * NUMTREES];


void Scene::init(int _width, int _height) {
	srand(time(0));
	width = _width;
	height = _height;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	window = glfwCreateWindow(width, height, "Tree Generator - 0000 FPS", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		system("PAUSE");
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		system("PAUSE");
		exit(-1);
	}
	initializeOpenGL();
	createShaders();

	camera = new Camera((float)width / (float)height, glm::vec3(0, 0, 0));
}
void Scene::deleteScene() {
	delete camera;
	glDeleteProgram(shader);
	glfwTerminate();
}
void Scene::initializeOpenGL() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glCullFace(GL_BACK);
	glfwSwapInterval(0);

	glViewport(0, 0, width, height);
	glClearColor(0.1f, 0.5f, 1.0f , 1);
	for (int i = 0; i < NUMTREES * NUMTREES; i++) {
		tree[i].init();
	}
}

void Scene::createShaders() {
	shader = createProgram("shaders/vert.glsl", "shaders/frag.glsl");
}

void Scene::renderFrame(float time, float deltaTime) {
	camera->updateInput(window, deltaTime);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(shader);

	setUniformFloat(shader, "iTime", float(glfwGetTime()));

	camera->setUniforms(shader, 1);
	camera->updateModel(shader);
	
	for (int i = 0; i < NUMTREES; i++) {
		for (int j = 0; j < NUMTREES; j++) {
			camera->identity();
			camera->translate(glm::fvec3(i * 20, 0, j * 20));
			camera->updateModel(shader);
			tree[i + j * NUMTREES].draw(shader);
		}
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Scene::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		for (int i = 0; i < NUMTREES * NUMTREES; i++) {
			tree[i].generateNewTree();
		}
	}
}

Camera* Scene::getCamera() {
	return camera;
}
GLFWwindow* Scene::getWindow() {
	return window;
}

float Scene::getAspectRatio() {
	return (float)width / (float)height;
}