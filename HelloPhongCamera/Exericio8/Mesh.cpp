#include <vector>

#include "Mesh.h"

void Mesh::initialize(GLuint VAO, int nVertices, GLuint texID, vector<float> coefficients, Shader* shader, glm::vec3 position, glm::vec3 scale, float angle, glm::vec3 axis) {
	this->VAO = VAO;
	this->nVertices = nVertices;
	this->texID = texID;
	this->coefficients = coefficients;
	this->shader = shader;
	this->position = position;
	this->scale = scale;
	this->angle = angle;
	this->axis = axis;
}

void Mesh::update() {
	glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(angle), axis);
	model = glm::scale(model, scale);
	shader->setMat4("model", glm::value_ptr(model));
	shader->setFloat("q", coefficients[0]);
	shader->setFloat("ka", coefficients[1]);
	shader->setFloat("kd", coefficients[2]);
	shader->setFloat("ks", coefficients[3]);
}

void Mesh::draw() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, nVertices);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
}