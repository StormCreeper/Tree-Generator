#include "Scene.h"

int Scene::width(800);
int Scene::height(600);
GLFWwindow* Scene::window(NULL);
Camera* Scene::camera(NULL);

unsigned int Scene::shader;
Tree Scene::tree;


void Scene::init(int _width, int _height) {
	width = _width;
	height = _height;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/
	glfwSwapInterval(0);

	glViewport(0, 0, width, height);
	glClearColor(0, 0, 0 , 1);

	tree.init();
}

void Scene::createShaders() {
	shader = createProgram("shaders/vert.glsl", "shaders/frag.glsl");
}

void Scene::renderFrame(float time, float deltaTime) {
	camera->updateInput(window, deltaTime);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(shader);

	setUniformFloat(shader, "iTime", glfwGetTime());

	camera->setUniforms(shader, 1);
	camera->updateModel(shader);

	tree.draw(shader);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Scene::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		
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