#ifndef CUBE_H
#define CUBE_H

#include "shader.h"
#include "textureHandler.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

class Cube 
{
public:
	glm::vec3 position;
	glm::vec3 CubeID;
	glm::vec3 size;
	float vertices[36*5];
	unsigned int VBO, VAO;
	glm::mat4 model;
	bool pX, nX, pY, nY, pZ, nZ;

	void init(glm::vec3 pos, const unsigned int &texture);
	void bufferize();
	void draw(Shader& shader);
private:
	unsigned char* textureData;
	unsigned int textureID;
};

#endif

#pragma once