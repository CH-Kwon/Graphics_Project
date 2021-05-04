/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Model.h
Purpose: Defining model functions
Language: c, c++
Platform: visual studio 2017, window
Project: chiho.kwon_CS300_3
Author: <name : Chiho Kwon, student login : chiho.kwon ,student id : 180003018
Creation date: 11/08/2018
End Header --------------------------------------------------------*/

#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <map>

class Camera;
class Shader;

class Model
{
	unsigned vao, vbo, ebo, normalbuffer;
	unsigned element_size;

public:

	Model();
	~Model();

	void Render(Camera* camera, Shader* shader, float aspect, glm::vec3 m_position);
	void Draw(std::vector<glm::vec3>& vert, std::vector<unsigned>& indices);

	bool LoadObj(const char* path);
	void loadTexture(std::vector<std::string> faces);
	void TextureData(Shader* shader, unsigned int& texture_ID);

	unsigned int textureID[6];
	int width, height;

	glm::vec3 position, scale, color;

	float rotation;

	std::vector<unsigned> vertex_indices;
	std::vector<glm::vec3> out_vertices;

	std::multimap<int, glm::vec3> faceNormal;
	std::multimap<int, glm::vec3> vertexNormal;
	std::vector<glm::vec3> normalVector;

};