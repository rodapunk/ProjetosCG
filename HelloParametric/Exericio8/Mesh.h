#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Mesh {
public:
	Mesh() {}
	~Mesh() {}
	void initialize(GLuint VAO, int nVertices, GLuint texID, vector<float> coefficients, Shader* shader, glm::vec3 position = glm::vec3(0.0, 0.0, 0.0), glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0), float angle = 0.0, glm::vec3 axis = glm::vec3(0.0, 0.0, 1.0), int positionIndex = 0, double previousTime = glfwGetTime());
	void update();
	void draw();
	void move(double currentTime, double velocity);

public:
	GLuint VAO;
	int nVertices;
	GLuint texID;
	vector<float> coefficients;	

	// Referência (endereço) do shader
	Shader* shader;

	// Informações sobre as transformações a serem aplicadas no objeto
	glm::vec3 position;
	glm::vec3 scale;
	float angle;
	glm::vec3 axis;
	vector<glm::vec3> positions;
	int positionIndex;
	double previousTime;
};