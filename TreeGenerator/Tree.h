#pragma once
#include <vector>
#include "utils.h"

class Tree {
	unsigned int VAO, VBO;
	unsigned int vertexCount;

	glm::mat4 transform;

	void pushVertex(std::vector<float>* vertices, float x, float y, float z, float nx, float ny, float nz, float l);
	void pushVertex(std::vector<float>* vertices, glm::fvec3 p, glm::fvec3 n, float l);
	void pushTriangle(std::vector<float>* vertices, glm::fvec3 &p1, glm::fvec3 &p2, glm::fvec3 &p3, float l);
	void pushTransformedTriangle(std::vector<float>* vertices, glm::fvec3 p1, glm::fvec3 p2, glm::fvec3 p3, float l);
	void pushTriangle(std::vector<float>* vertices, glm::fvec3& p1, glm::fvec3& p2, glm::fvec3& p3, glm::fvec3& n1, glm::fvec3& n2, glm::fvec3& n3, float l);
	void pushTransformedTriangle(std::vector<float>* vertices, glm::fvec3& p1, glm::fvec3& p2, glm::fvec3& p3, glm::fvec3& n1, glm::fvec3& n2, glm::fvec3& n3, float l);
	void branch(std::vector<float>* vertices, glm::fvec3 position, glm::fquat rotation, float length, float depth);

public:
	void init();
	void update(float deltaTime);
	void draw(unsigned int shader);

	void addCylinder(std::vector<float>* vertices, float height, float radius);

	void generateNewTree();
	
	~Tree();
};

