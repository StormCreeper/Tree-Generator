#include "Scene.h"
#include <ctime>

constexpr auto NUMTREES = 1;

int Scene::width(800);
int Scene::height(600);
GLFWwindow* Scene::window(nullptr);
Camera* Scene::camera(nullptr);

unsigned int Scene::shader;
Tree Scene::tree[NUMTREES * NUMTREES];


void Scene::init(int _width, int _height) {
	std::cout << "Welcome to this project\nThis is a tree generator, using a fractal approach, each branch make other branches of a smaller size grow on it, in a recursive pattern.\nThe rendering is done by a custom engine (made during the jam) using modern OpenGL techniques.\nUsage:\nWASD to move, mouse to rotate the camera, click to generate a new tree and escape to exit\n";
	system("pause");
	srand(time(nullptr));
	width = _width;
	height = _height;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	window = glfwCreateWindow(width, height, "Tree Generator - 0000 FPS", nullptr, nullptr);
	if (window == nullptr) {
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

	camera = new Camera((float)width / (float)height, glm::vec3(0, 12, -20));
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
	for (auto & t : tree) {
		t.init();
	}
}

void Scene::createShaders() {
	shader = createProgram("shaders/vert.glsl", "shaders/frag.glsl");
}

void Scene::renderFrame(float time, float deltaTime) {
	glfwPollEvents();
	camera->updateInput(window, deltaTime);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(shader);

	setUniformFloat(shader, "iTime", float(glfwGetTime()));

	camera->setUniforms(shader, true);
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
}

void Scene::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		for (auto & t : tree) {
			t.generateNewTree();
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