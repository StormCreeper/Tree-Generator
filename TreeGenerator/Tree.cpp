#include "Tree.h"
#include <random>
#include <time.h>
#include <glm/gtx/quaternion.hpp>

const float PI = 3.1415926535f;

void Tree::init() {

	srand(time(0));

	std::vector<float> vertices;

	/*float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};*/

	/*pushVertex(&vertices, -0.5f, -0.5f, 0.0f);
	pushVertex(&vertices, 0.5f, -0.5f, 0.0f);
	pushVertex(&vertices, 0.0f, 0.5f, 0.0f);*/

	glm::fquat rotation = glm::fquat(glm::vec3(-PI / 2.0f, 0, 0));
	glm::fvec3 position = glm::fvec3(0, 0, 0);

	branch(&vertices, position, rotation, 8, 8);

	float *verticesArray = (float*) malloc(vertices.size() * sizeof(float));
	std::copy(vertices.begin(), vertices.end(), verticesArray);

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), verticesArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	vertexCount = vertices.size();

	std::cout << vertexCount * sizeof(float) / 1000 << " kb sent to the GPU" << std::endl;
	
	free(verticesArray);
}

void Tree::branch(std::vector<float>* vertices, glm::fvec3 position, glm::fquat rotation, float length, float depth) {
	if (depth) {
		glm::fvec3 point1 = position + glm::fvec3(0, 0, length);
		glm::fvec3 point2 = position + glm::fvec3(0, length / 8, 0);
		glm::fvec3 point3 = position - glm::fvec3(0, length / 8, 0);
		point1 = position + (rotation * (point1 - position));
		point2 = position + (rotation * (point2 - position));
		point3 = position + (rotation * (point3 - position));

		transform = glm::identity<glm::mat4>();
		transform = glm::translate(transform, position);
		transform *= glm::toMat4(rotation);

		addCylinder(vertices);

		pushTriangle(vertices, point1, point2, point3);

		if (rand() % 4) branch(vertices, point1, rotation * glm::fquat(glm::fvec3(PI / 8.0f, 0, 0)), length * 0.8f, depth - 1);
		if (rand() % 4) branch(vertices, point1, rotation * glm::fquat(glm::fvec3(-PI / 8.0f, 0, 0)), length * 0.8f, depth - 1);
		if (rand() % 4) branch(vertices, point1, rotation * glm::fquat(glm::fvec3(0, PI / 8.0f, 0)), length * 0.8f, depth - 1);
		if (rand() % 4) branch(vertices, point1, rotation * glm::fquat(glm::fvec3(0, -PI / 8.0f, 0)), length * 0.8f, depth - 1);
	}
}
void Tree::update(float deltaTime) {
}
void Tree::draw(unsigned int shader) {
	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount / 3);
}
void Tree::addCylinder(std::vector<float>* vertices) {
	int n = 10;
	float r = 1;
	float height = 2;

	glm::fvec3 c1(0, height / 2.0f, 0);
	glm::fvec3 c2(0, -height / 2.0f, 0);

	glm::fvec3 nc1(0, 1, 0);
	glm::fvec3 nc2(0, -1, 0);

	for (float a = 0; a < PI * 2; a += PI * 2 / float(n)) {
		glm::fvec3 p1(r * cosf(a), height / 2.0f, r * sinf(a));
		glm::fvec3 p2(r * cosf(a), - height / 2.0f, r * sinf(a));

		glm::fvec3 p1p(r * cosf(a + PI * 2 / float(n)), height / 2.0f, r * sinf(a + PI * 2 / float(n)));
		glm::fvec3 p2p(r * cosf(a + PI * 2 / float(n)), -height / 2.0f, r * sinf(a + PI * 2 / float(n)));

		glm::fvec3 norm = glm::normalize(p1 - c1);
		glm::fvec3 normP = glm::normalize(p1p - c1);

		pushTransformedTriangle(vertices, c1, p1p, p1);// , nc1, nc1, nc1);
		pushTransformedTriangle(vertices, c2, p2, p2p);// , nc2, nc2, nc2);
		pushTransformedTriangle(vertices, p1, p2p, p2);// , norm, normP, norm);
		pushTransformedTriangle(vertices, p2p, p1, p1p);// , normP, norm, normP);
	}
}
void Tree::pushTriangle(std::vector<float>* vertices, glm::fvec3 &p1, glm::fvec3 &p2, glm::fvec3 &p3) {
	glm::fvec3 norm = glm::normalize(glm::cross(p3 - p1, p2 - p1));

	pushVertex(vertices, p1, norm);
	pushVertex(vertices, p2, norm);
	pushVertex(vertices, p3, norm);
}
void Tree::pushTransformedTriangle(std::vector<float>* vertices, glm::fvec3 p1, glm::fvec3 p2, glm::fvec3 p3) {
	p1 = glm::fvec3(transform * glm::fvec4(p1, 1.0f));
	p2 = glm::fvec3(transform * glm::fvec4(p2, 1.0f));
	p3 = glm::fvec3(transform * glm::fvec4(p3, 1.0f));

	glm::fvec3 norm = glm::normalize(glm::cross(p3 - p1, p2 - p1));
	
	pushVertex(vertices, p1, norm);
	pushVertex(vertices, p2, norm);
	pushVertex(vertices, p3, norm);
}
void Tree::pushTriangle(std::vector<float>* vertices, glm::fvec3& p1, glm::fvec3& p2, glm::fvec3& p3, glm::fvec3& n1, glm::fvec3& n2, glm::fvec3& n3) {
	pushVertex(vertices, p1, n1);
	pushVertex(vertices, p2, n2);
	pushVertex(vertices, p3, n3);
}
void Tree::pushTransformedTriangle(std::vector<float>* vertices, glm::fvec3& p1, glm::fvec3& p2, glm::fvec3& p3, glm::fvec3& n1, glm::fvec3& n2, glm::fvec3& n3) {
	pushVertex(vertices, glm::fvec3(transform * glm::fvec4(p1, 1.0f)), glm::fvec3(transform * glm::fvec4(n1, 1.0f)));
	pushVertex(vertices, glm::fvec3(transform * glm::fvec4(p2, 1.0f)), glm::fvec3(transform * glm::fvec4(n2, 1.0f)));
	pushVertex(vertices, glm::fvec3(transform * glm::fvec4(p3, 1.0f)), glm::fvec3(transform * glm::fvec4(n3, 1.0f)));
}
void Tree::pushVertex(std::vector<float>* vertices, glm::fvec3 p, glm::fvec3 n) {
	pushVertex(vertices, p.x, p.y, p.z, n.x, n.y, n.z);
}
void Tree::pushVertex(std::vector<float> *vertices, float x, float y, float z, float nx, float ny, float nz) {
	vertices->push_back(x); vertices->push_back(y); vertices->push_back(z);
	vertices->push_back(nx); vertices->push_back(ny); vertices->push_back(nz);
}

Tree::~Tree() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}
