/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Model.h
Purpose: Defining model functions
Language: c, c++
Platform: visual studio 2017, window
Project: chiho.kwon_CS350_1
Author: <name : Chiho Kwon, student login : chiho.kwon ,student id : 180003018
Creation date: 2/3/2019
End Header --------------------------------------------------------*/

#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <algorithm>
#include <map>

class Camera;
class Shader;

class Model
{
	unsigned vao, vbo, ebo, tbo, normalbuffer;
	unsigned element_size;
	float Max_x, Min_x, Max_y, Min_y, Max_z, Min_z;
	unsigned width, height;
	unsigned tex1, tex2;
	bool isUseTex;

public:

	Model();
	~Model();

	int ProjectionType;
	int NormalCalculation;

	void Render(Camera* camera, Shader* shader, float aspect, glm::vec3 m_position);
	void Draw(std::vector<glm::vec3>& vert, std::vector<unsigned>& indices, std::vector<glm::vec2> textures);

	bool LoadObj(const char* path, bool isUseTex, glm::vec3 midpoint);

	glm::vec3 position, scale, color;

	float rotation;

	std::vector<unsigned> vertex_indices;
	std::vector<glm::vec3> out_vertices;

	std::multimap<int, glm::vec3> faceNormal;
	std::multimap<int, glm::vec3> vertexNormal;
	std::vector<glm::vec3> normalVector;

	std::vector<glm::vec2> textureUV;
	std::vector<glm::vec3> TextSpecInfo;
	std::vector<glm::vec3> TextDiffInfo;

	bool loadPPM(const char* path, std::vector<glm::vec3>& values_);
	void TextureData(Shader* shader);
	void TextureProcess();
};