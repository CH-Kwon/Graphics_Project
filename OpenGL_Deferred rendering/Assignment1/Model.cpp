/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Model.cpp
Purpose: For modeling object using obj loader
Language: c, c++
Platform: visual studio 2017, window
Project: chiho.kwon_CS350_1
Author: <name : Chiho Kwon, student login : chiho.kwon ,student id : 180003018
Creation date: 2/3/2019
End Header --------------------------------------------------------*/

#define PIEDOUBLE 6.283185308
#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"

#include "Model.h"
#include "Camera.h"
#include "Shader.h"

#include <iostream>

Model::Model()
	: position(glm::vec3()), scale(glm::vec3(1, 1, 1)), color(glm::vec3(1.0f, 1.0f, 1.0f)),
	rotation(0.f),width(512),height(512),isUseTex(false)
{
}

Model::~Model()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteVertexArrays(1, &vbo);
	glDeleteVertexArrays(1, &ebo);
	glDeleteVertexArrays(1, &tbo);
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &tbo);
}

void Model::Render(Camera* camera, Shader* shader, float aspect, glm::vec3 m_position)
{
	const static glm::vec3 up(0, 1, 0);

	glm::mat4 identity_translate(1.0);
	glm::mat4 identity_scale(1.0);
	glm::mat4 identity_rotation(1.0);
	glm::mat4 model = glm::translate(identity_translate, m_position) * glm::scale(identity_scale, scale) * glm::rotate(identity_rotation, rotation, up);
	glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), aspect, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();

	if (isUseTex)
	{
		shader->SetInt("projtype", ProjectionType);
		shader->SetInt("NormalCalculation", NormalCalculation);
	}
	shader->SetMat4("model", model);
	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);
	shader->SetVec3("color", color);

	glBindVertexArray(vao);

	if (isUseTex)
	{
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(textureUV.size() * sizeof(glm::vec2)), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, element_size, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
}

void Model::Draw(std::vector<glm::vec3>& vert, std::vector<unsigned>& indices, std::vector<glm::vec2> textures)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenBuffers(1, &tbo);

	glGenBuffers(1, &normalbuffer);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(glm::vec3), &vert[0], GL_STATIC_DRAW);
	

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normalVector.size() * sizeof(glm::vec3), &normalVector[0], GL_STATIC_DRAW);
	if (isUseTex)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), &textures[0], GL_STATIC_DRAW);
	}
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

	if (isUseTex)
	{
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, tbo);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	element_size = indices.size() * sizeof(unsigned);
}

bool Model::LoadObj(const char * path, bool isUseTex, glm::vec3 midpoint)
{
	float x_max = 0,x_min = 0,y_max = 0,y_min = 0,z_max = 0,z_min = 0,max_abs = 0;

	glm::vec3 P1, P2;
	glm::vec3 face_Normal;

	FILE* file;
	fopen_s(&file, path, "r");

	if (file == NULL)
	{
		std::cout << "Cannot open the file\n";
		return false;
	}
	while (true)
	{
		char lineHeader[128];

		int result = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader) / sizeof(char));
		if (result == EOF)
		{
			faceNormal.clear();
			for (unsigned int i = 0; i < vertex_indices.size(); i += 3)
			{
				P1 = (out_vertices[vertex_indices[i + 2]] - out_vertices[vertex_indices[i + 1]]);
				P2 = (out_vertices[vertex_indices[i + 1]] - out_vertices[vertex_indices[i]]);

				face_Normal = glm::normalize(glm::cross(P1, P2));
				for (int j = 0; j < 3; j++)
				{
					bool isExist = false;
					auto it = faceNormal.equal_range(vertex_indices[i + j]);
					for (auto iter = it.first; iter != it.second; ++iter)
					{
						if (iter->second == face_Normal)
						{
							isExist = true;
							break;
						}
					}
					if (isExist == false)
					{
						faceNormal.emplace(vertex_indices[i + j], face_Normal);
					}
				}
			}

			vertexNormal = faceNormal;

			normalVector.clear();
			normalVector.resize(out_vertices.size());

			for (auto& normal : vertexNormal)
			{
				normalVector[normal.first] += normal.second;
			}
			for (auto& normal : normalVector)
			{
				normal = glm::normalize(normal);
			}

			Max_x = x_max;
			Min_x = x_min;
			Max_y = y_max;
			Min_y = y_min;

			if (max_abs < glm::abs(x_max - x_min))
				max_abs = glm::abs(x_max - x_min);
			if (max_abs < glm::abs(y_max - y_min))
				max_abs = glm::abs(y_max - y_min);
			if (max_abs < glm::abs(z_max - z_min))
				max_abs = glm::abs(z_max - z_min);

			midpoint = glm::vec3((x_max + x_min) / (2 * max_abs),(y_max + y_min) / (2 * max_abs), (z_max + z_min) / (2 * max_abs));

			Max_x /= (0.5f * max_abs); 	Min_x /= (0.5f * max_abs);	
			Max_y /= (0.5f * max_abs);	Min_y /= (0.5f * max_abs);
			Max_z /= (0.5f * max_abs);	Min_z /= (0.5f * max_abs);
			Max_x -= (2.f * midpoint.x); Min_x -= (2.f * midpoint.x);
			Max_y -= (2.f * midpoint.y); Min_y -= (2.f * midpoint.y);
			Max_z -= (2.f * midpoint.z); Min_z -= (2.f * midpoint.z);

			for (unsigned i = 0; i < out_vertices.size(); ++i)
			{
				out_vertices[i].x = out_vertices[i].x / (0.5f * max_abs);
				out_vertices[i].y = out_vertices[i].y / (0.5f * max_abs);
				out_vertices[i].z = out_vertices[i].z / (0.5f * max_abs);
				out_vertices[i] = out_vertices[i] - (2.f * midpoint);
				if (isUseTex)
				{
					glm::vec3 normalized = glm::normalize(out_vertices[i]);

					float theta = glm::atan(normalized.y / normalized.x);
					glm::vec2 textUV = glm::vec2(theta / PIEDOUBLE, (normalized.z + 1) * 0.5f);

					textureUV.push_back(textUV);
				}
			}
			break;
		}
		if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
		
			if (x_min < vertex.x)
				x_min = vertex.x;
			if (y_min < vertex.y)
				y_min = vertex.y;
			if (z_min < vertex.z)
				z_min = vertex.z;

			if (x_max > vertex.x)
				x_max = vertex.x;
			if (y_max > vertex.y)
				y_max = vertex.y;
			if (z_max > vertex.z)
				z_max = vertex.z;

			out_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			unsigned int vertexIndex[3];
			int matches = fscanf_s(file, "%d %d %d\n", &vertexIndex[0],
				&vertexIndex[1], &vertexIndex[2]);
			if (matches != 3)
			{
				std::cout << "File can't be read\n";
				return false;
			}
			vertex_indices.push_back(vertexIndex[0] - 1);
			vertex_indices.push_back(vertexIndex[1] - 1);
			vertex_indices.push_back(vertexIndex[2] - 1);
		}
	}
	return true;
}

bool Model::loadPPM(const char* path, std::vector<glm::vec3>& values_)
{
	FILE* file2;
	fopen_s(&file2, path, "r");

	if (file2 == NULL)
	{
		printf("File cannot be opened !\n");
		return false;
	}
	char fileHeader[128] = { 0 };
	int res = fscanf_s(file2, "%s\n", &fileHeader, sizeof(fileHeader)/sizeof(char));
	res = fscanf_s(file2, "%d %d\n", &width, &height);
	while (1)
	{
		int x = 0, y = 0, z = 0;
		int end = fscanf_s(file2, "%d\n%d\n%d", &x, &y, &z);
		if (end == EOF)
			break;

		glm::vec3 vec_(static_cast<float>(x / 255.f), static_cast<float>(y / 255.f), static_cast<float>(z / 255.f));

		values_.push_back(vec_);
	}
	return true;
}

void Model::TextureProcess()
{
	glGenTextures(1, &tex1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT,
		(const GLfloat *)TextDiffInfo.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glGenTextures(1, &tex2);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT,
		(const GLfloat *)TextSpecInfo.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	return;
}

void Model::TextureData(Shader* shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
	GLint texSamplerLoc_1 = glGetUniformLocation(shader->m_programId, "Texture_1");
	glUniform1i(texSamplerLoc_1, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);
	GLint texSamplerLoc_2 = glGetUniformLocation(shader->m_programId, "Texture_2");
	glUniform1i(texSamplerLoc_2, 1);
}




