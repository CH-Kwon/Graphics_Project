/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Model.cpp
Purpose: For modeling object using obj loader
Language: c, c++
Platform: visual studio 2017, window
Project: chiho.kwon_CS300_3
Author: <name : Chiho Kwon, student login : chiho.kwon ,student id : 180003018
Creation date: 11/08/2018
End Header --------------------------------------------------------*/
#define STB_IMAGE_IMPLEMENTATION
#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "STB/stb_image.h"

#include "Model.h"
#include "Camera.h"
#include "Shader.h"

#include <iostream>

Model::Model()
	: position(glm::vec3()), scale(glm::vec3(1, 1, 1)), color(glm::vec3(1.0f, 1.0f, 1.0f)),
	rotation(0.f)
{
}

Model::~Model()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
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
	glm::mat4 viewtwo = glm::mat4(glm::mat3(camera->GetViewMatrix()));

	shader->SetMat4("model", model);
	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);
	shader->SetMat4("viewtwo", viewtwo);
	shader->SetVec3("color", color);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, element_size, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Model::Draw(std::vector<glm::vec3>& vert, std::vector<unsigned>& indices)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glGenBuffers(1, &normalbuffer);
	
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(glm::vec3), &vert[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normalVector.size() * sizeof(glm::vec3), &normalVector[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	element_size = indices.size() * sizeof(unsigned);
}

bool Model::LoadObj(const char * path)
{
	float x_max = 0,x_min = 0,y_max = 0,y_min = 0,z_max = 0,z_min = 0,max_abs = 0;
	glm::vec3 mid(0,0,0);

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

			if (max_abs < glm::abs(x_max - x_min))
				max_abs = glm::abs(x_max - x_min);
			if (max_abs < glm::abs(y_max - y_min))
				max_abs = glm::abs(y_max - y_min);
			if (max_abs < glm::abs(z_max - z_min))
				max_abs = glm::abs(z_max - z_min);

			mid = glm::vec3((x_max + x_min) / (2 * max_abs),(y_max + y_min) / (2 * max_abs), (z_max + z_min) / (2 * max_abs));

			for (unsigned i = 0; i < out_vertices.size(); ++i)
			{
				out_vertices[i].x = out_vertices[i].x / (0.5f * max_abs);
				out_vertices[i].y = out_vertices[i].y / (0.5f * max_abs);
				out_vertices[i].z = out_vertices[i].z / (0.5f * max_abs);
				out_vertices[i]   = out_vertices[i] - (2.f * mid);
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

void Model::loadTexture(std::vector<std::string> faces)
{
	int nrComponents0;
	glGenTextures(1, &textureID[0]);
	unsigned char *data0 = stbi_load(faces[0].c_str(), &width, &height, &nrComponents0, 0);
	if (data0)
	{
		GLenum format;
		if (nrComponents0 == 1)
			format = GL_RED;
		else if (nrComponents0 == 3)
			format = GL_RGB;
		else if (nrComponents0 == 4)
			format = GL_RGBA;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data0);
		glGenerateMipmap(GL_TEXTURE_2D);
		 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data0);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << faces[0].c_str() << std::endl;
		stbi_image_free(data0);
	}

	glGenTextures(1, &textureID[1]);

	int nrComponents1;
	unsigned char *data1 = stbi_load(faces[1].c_str(), &width, &height, &nrComponents1, 0);
	if (data1)
	{
		GLenum format;
		if (nrComponents1 == 1)
			format = GL_RED;
		else if (nrComponents1 == 3)
			format = GL_RGB;
		else if (nrComponents1 == 4)
			format = GL_RGBA;
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureID[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data1);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << faces[1].c_str() << std::endl;
		stbi_image_free(data1);
	}

	glGenTextures(1, &textureID[2]);

	int nrComponents2;
	unsigned char *data2 = stbi_load(faces[2].c_str(), &width, &height, &nrComponents2, 0);
	if (data2)
	{
		GLenum format;
		if (nrComponents2 == 1)
			format = GL_RED;
		else if (nrComponents2 == 3)
			format = GL_RGB;
		else if (nrComponents2 == 4)
			format = GL_RGBA;
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureID[2]);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data2);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << faces[2].c_str() << std::endl;
		stbi_image_free(data2);
	}

	glGenTextures(1, &textureID[3]);

	int nrComponents3;
	unsigned char *data3 = stbi_load(faces[3].c_str(), &width, &height, &nrComponents3, 0);
	if (data3)
	{
		GLenum format;
		if (nrComponents3 == 1)
			format = GL_RED;
		else if (nrComponents3 == 3)
			format = GL_RGB;
		else if (nrComponents3 == 4)
			format = GL_RGBA;
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textureID[3]);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data3);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data3);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << faces[3].c_str() << std::endl;
		stbi_image_free(data3);
	}

	glGenTextures(1, &textureID[4]);

	int nrComponents4;
	unsigned char *data4 = stbi_load(faces[4].c_str(), &width, &height, &nrComponents4, 0);
	if (data4)
	{
		GLenum format;
		if (nrComponents4 == 1)
			format = GL_RED;
		else if (nrComponents4 == 3)
			format = GL_RGB;
		else if (nrComponents4 == 4)
			format = GL_RGBA;
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, textureID[4]);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data4);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data4);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << faces[4].c_str() << std::endl;
		stbi_image_free(data4);
	}

	glGenTextures(1, &textureID[5]);

	int nrComponents5;
	unsigned char *data5 = stbi_load(faces[5].c_str(), &width, &height, &nrComponents5, 0);
	if (data5)
	{
		GLenum format;
		if (nrComponents5 == 1)
			format = GL_RED;
		else if (nrComponents5 == 3)
			format = GL_RGB;
		else if (nrComponents5 == 4)
			format = GL_RGBA;
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, textureID[5]);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data5);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data5);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << faces[5].c_str() << std::endl;
		stbi_image_free(data5);
	}
}

void Model::TextureData(Shader* shader, unsigned int& texture_ID)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	shader->SetInt("skybox[0]", 0);
	/*GLint texSamplerLoc_0 = glGetUniformLocation(shader->m_programId, "skybox[0]");
	glUniform1i(texSamplerLoc_0, 0);*/

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_ID+1);
	shader->SetInt("skybox[1]", 1);
	/*GLint texSamplerLoc_1 = glGetUniformLocation(shader->m_programId, "skybox[1]");
	glUniform1i(texSamplerLoc_1, 1);*/

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture_ID+2);
	shader->SetInt("skybox[2]", 2);
	/*GLint texSamplerLoc_2 = glGetUniformLocation(shader->m_programId, "skybox[2]");
	glUniform1i(texSamplerLoc_2, 2);*/

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture_ID+3);
	shader->SetInt("skybox[3]", 3);
	/*GLint texSamplerLoc_3 = glGetUniformLocation(shader->m_programId, "skybox[3]");
	glUniform1i(texSamplerLoc_3, 3);*/

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, texture_ID+4);
	shader->SetInt("skybox[4]", 4);
	/*GLint texSamplerLoc_4 = glGetUniformLocation(shader->m_programId, "skybox[4]");
	glUniform1i(texSamplerLoc_4, 4);*/

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texture_ID+5);
	shader->SetInt("skybox[5]", 5);
	/*GLint texSamplerLoc_5 = glGetUniformLocation(shader->m_programId, "skybox[5]");
	glUniform1i(texSamplerLoc_5, 5);*/
}
