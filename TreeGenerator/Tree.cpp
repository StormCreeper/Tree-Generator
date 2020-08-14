#include "Tree.h"
#include <random>
#include <glm/gtx/quaternion.hpp>

const float PI = 3.1415926535f;

void Tree::init() {

	std::vector<float> vertices;

	glm::fquat rotation = glm::fquat(glm::vec3(-PI / 2.0f, 0, 0));
	glm::fvec3 position = glm::fvec3(0, 0, 0);

	glm::fvec3 p1(-20, 0, -20);
	glm::fvec3 p2(-20, 0,  20);
	glm::fvec3 p3( 20, 0,  20);
	glm::fvec3 p4( 20, 0, -20);

	pushTriangle(&vertices, p1, p2, p3, 1.0f);
	pushTriangle(&vertices, p1, p3, p4, 1.0f);
	
	branch(&vertices, position, rotation, 8, 11);

	float *verticesArray = (float*) malloc(vertices.size() * sizeof(float));
	std::copy(vertices.begin(), vertices.end(), verticesArray);

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), verticesArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	vertexCount = vertices.size();

	//std::cout << vertexCount * sizeof(float) / 1000 << " kb sent to the GPU" << std::endl;
	
	free(verticesArray);
}

void Tree::branch(std::vector<float>* vertices, glm::fvec3 position, glm::fquat rotation, float length, float depth) {
	if (depth) {
		glm::fvec3 point1 = position + glm::fvec3(0, 0, length);
		point1 = position + (rotation * (point1 - position));

		transform = glm::identity<glm::mat4>();
		transform = glm::translate(transform, position);
		transform *= glm::toMat4(rotation);
		transform = glm::translate(transform, glm::fvec3(0, 0, length / 2));
		transform = glm::rotate(transform, PI / 2.0f, glm::fvec3(1, 0, 0));

		addCylinder(vertices, length, length / 8.0f);

		float nbranches = rand() % 5 + 1;

		float offsetAngle = (rand() % 100) / 100.0f * PI * 2.0f;
		
		for (int i = 0; i < nbranches; i++) {
			branch(vertices, point1, rotation * glm::fquat(glm::fvec3(cosf(i / nbranches * PI * 2.0f + offsetAngle) * 0.5f, sinf(i / nbranches * PI * 2.0f + offsetAngle) * 0.5f, 0)), length * (0.7f + (rand() % 100) / 200.0f - 0.25f), depth - 1);
		}
	}
}
void Tree::update(float deltaTime) {
}
void Tree::draw(unsigned int shader) {
	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount / 3);
}
void Tree::addCylinder(std::vector<float>* vertices, float height, float radius) {
	int n = std::max(height * 4.0f, 3.0f);

	glm::fvec3 c1(0, height / 2.0f, 0);
	glm::fvec3 c2(0, -height / 2.0f, 0);

	for (float a = 0; a < PI * 2; a += PI * 2 / float(n)) {
		glm::fvec3 p1(radius * cosf(a), height / 2.0f, radius * sinf(a));
		glm::fvec3 p2(radius * cosf(a), - height / 2.0f, radius * sinf(a));

		glm::fvec3 p1p(radius * cosf(a + PI * 2 / float(n)), height / 2.0f, radius * sinf(a + PI * 2 / float(n)));
		glm::fvec3 p2p(radius * cosf(a + PI * 2 / float(n)), -height / 2.0f, radius * sinf(a + PI * 2 / float(n)));

		pushTransformedTriangle(vertices, c1, p1p, p1, 1 - height / 8.0f);
		pushTransformedTriangle(vertices, c2, p2, p2p, 1 - height / 8.0f);
		pushTransformedTriangle(vertices, p1, p2p, p2, 1 - height / 8.0f);
		pushTransformedTriangle(vertices, p2p, p1, p1p, 1 - height / 8.0f);
	}
}
void Tree::pushTriangle(std::vector<float>* vertices, glm::fvec3 &p1, glm::fvec3 &p2, glm::fvec3 &p3, float l) {
	glm::fvec3 norm = glm::normalize(glm::cross(p3 - p1, p2 - p1));

	pushVertex(vertices, p1, norm, l);
	pushVertex(vertices, p2, norm, l);
	pushVertex(vertices, p3, norm, l);
}
void Tree::pushTransformedTriangle(std::vector<float>* vertices, glm::fvec3 p1, glm::fvec3 p2, glm::fvec3 p3, float l) {
	p1 = glm::fvec3(transform * glm::fvec4(p1, 1.0f));
	p2 = glm::fvec3(transform * glm::fvec4(p2, 1.0f));
	p3 = glm::fvec3(transform * glm::fvec4(p3, 1.0f));

	glm::fvec3 norm = glm::normalize(glm::cross(p3 - p1, p2 - p1));
	
	pushVertex(vertices, p1, norm, l);
	pushVertex(vertices, p2, norm, l);
	pushVertex(vertices, p3, norm, l);
}
void Tree::pushTriangle(std::vector<float>* vertices, glm::fvec3& p1, glm::fvec3& p2, glm::fvec3& p3, glm::fvec3& n1, glm::fvec3& n2, glm::fvec3& n3, float l) {
	pushVertex(vertices, p1, n1, l);
	pushVertex(vertices, p2, n2, l);
	pushVertex(vertices, p3, n3, l);
}
void Tree::pushTransformedTriangle(std::vector<float>* vertices, glm::fvec3& p1, glm::fvec3& p2, glm::fvec3& p3, glm::fvec3& n1, glm::fvec3& n2, glm::fvec3& n3, float l) {
	pushVertex(vertices, glm::fvec3(transform * glm::fvec4(p1, 1.0f)), glm::fvec3(transform * glm::fvec4(n1, 1.0f)), l);
	pushVertex(vertices, glm::fvec3(transform * glm::fvec4(p2, 1.0f)), glm::fvec3(transform * glm::fvec4(n2, 1.0f)), l);
	pushVertex(vertices, glm::fvec3(transform * glm::fvec4(p3, 1.0f)), glm::fvec3(transform * glm::fvec4(n3, 1.0f)), l);
}
void Tree::pushVertex(std::vector<float>* vertices, glm::fvec3 p, glm::fvec3 n, float l) {
	pushVertex(vertices, p.x, p.y, p.z, n.x, n.y, n.z, l);
}
void Tree::pushVertex(std::vector<float> *vertices, float x, float y, float z, float nx, float ny, float nz, float l) {
	vertices->push_back(x); vertices->push_back(y); vertices->push_back(z);
	vertices->push_back(nx); vertices->push_back(ny); vertices->push_back(nz);
	vertices->push_back(l);
}

void Tree::generateNewTree() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	init();
}

Tree::~Tree() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}
