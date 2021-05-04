/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Model.cpp
Purpose: For modeling object using obj loader
Language: c, c++
Platform: visual studio 2017, window
Project: chiho.kwon_CS350_2
Author: <name : Chiho Kwon, student login : chiho.kwon ,student id : 180003018
Creation date: 2/24/2019
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
	rotation(0.f),width(512),height(512),isUseTex(false), isUseBV(false),pca_mid(0)
	, x_minpt(0), x_maxpt(0), y_maxpt(0), y_minpt(0), z_minpt(0),z_maxpt(0), 
	isUseRotation(false)
{
}

Model::~Model()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteVertexArrays(1, &o_vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &tbo);
	glDeleteBuffers(1, &o_vbo);
	glDeleteBuffers(1, &o_ebo);
	glDeleteBuffers(1, &color_buffer);
}
void Model::RenderPartitionColor(Camera* camera, Shader* shader, float aspect, glm::vec3 m_position)
{
	const static glm::vec3 up(0, 1, 0);

	glm::mat4 identity_translate(1.0);
	glm::mat4 identity_scale(1.0);
	glm::mat4 identity_rotation(1.0);
	glm::mat4 model = glm::translate(identity_translate, m_position) * glm::rotate(identity_rotation, rotation, up) * glm::scale(identity_scale, scale);
	glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), aspect, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();

	shader->SetMat4("model", model);
	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);

	glGetIntegerv(GL_POLYGON_MODE, &poly);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(o_vao);
	glDrawElements(GL_TRIANGLES, element_size, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, poly);
}
void Model::RenderBspColor(Camera * camera, Shader * shader, float aspect, glm::vec3 m_position)
{
	const static glm::vec3 up(0, 1, 0);

	glm::mat4 identity_translate(1.0);
	glm::mat4 identity_scale(1.0);
	glm::mat4 identity_rotation(1.0);
	glm::mat4 model = glm::translate(identity_translate, m_position) * glm::rotate(identity_rotation, rotation, up) * glm::scale(identity_scale, scale);
	glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), aspect, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();

	shader->SetMat4("model", model);
	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);

	glGetIntegerv(GL_POLYGON_MODE, &poly);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(b_vao);
	glDrawElements(GL_TRIANGLES, element_size, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, poly);
}
void Model::Render(Camera* camera, Shader* shader, float aspect, glm::vec3 m_position, bool isUseBV, bool isUseRotation )
{
	const static glm::vec3 up(0, 1, 0);

	glm::mat4 identity_translate(1.0);
	glm::mat4 identity_scale(1.0);
	glm::mat4 identity_rotation(1.0);
	glm::mat4 model;
	if (!isUseRotation)
	{
		model = glm::translate(identity_translate, m_position) * glm::rotate(identity_rotation, rotation, up) * glm::scale(identity_scale, scale);
	}
	else
	{
		model = glm::translate(identity_translate, m_position) * glm::mat4(m_v) *glm::scale(identity_scale, scale);
	}
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
		if (isUseBV)
		{
			glDrawElements(GL_LINE_STRIP, element_size, GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, element_size, GL_UNSIGNED_INT, nullptr);
		}
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

void Model::DrawOctree(std::vector<glm::vec3>& vert, std::vector<unsigned>& indices, std::vector<glm::vec2> textures)
{
	glGenVertexArrays(1, &o_vao);
	glGenBuffers(1, &o_vbo);
	glGenBuffers(1, &o_ebo);
	glGenBuffers(1, &color_buffer);

	glBindVertexArray(o_vao);

	glBindBuffer(GL_ARRAY_BUFFER, o_vbo);
	glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(glm::vec3), &vert[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, octree_color.size() * sizeof(glm::vec3), &octree_color[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, o_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	element_size = indices.size() * sizeof(unsigned);
}

void Model::DrawBsptree(std::vector<glm::vec3>& vert, std::vector<unsigned>& indices, std::vector<glm::vec2> textures)
{
	glGenVertexArrays(1, &b_vao);
	glGenBuffers(1, &b_vbo);
	glGenBuffers(1, &b_ebo);
	glGenBuffers(1, &bsp_buffer);

	glBindVertexArray(b_vao);

	glBindBuffer(GL_ARRAY_BUFFER, b_vbo);
	glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(glm::vec3), &vert[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, bsp_buffer);
	glBufferData(GL_ARRAY_BUFFER, bsptree_color.size() * sizeof(glm::vec3), &bsptree_color[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, b_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, bsp_buffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	element_size = indices.size() * sizeof(unsigned);
}

bool Model::LoadObj(const char * path, bool isUseTex, bool isUseBV, glm::vec3 midpoint)
{
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

				Triangle temp_tri;
				temp_tri.normal = face_Normal;
				temp_tri.pt[0] = out_vertices[vertex_indices[i]];
				temp_tri.pt[1] = out_vertices[vertex_indices[i+1]];
				temp_tri.pt[2] = out_vertices[vertex_indices[i+2]];

				m_triangle.push_back(temp_tri);

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
			Max_z = z_max;
			Min_z = z_min;

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

			//get object scale AABB	

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
		
			if (x_min > vertex.x)
				x_min = vertex.x;
			if (y_min > vertex.y)
				y_min = vertex.y;
			if (z_min > vertex.z)
				z_min = vertex.z;

			if (x_max < vertex.x)
				x_max = vertex.x;
			if (y_max < vertex.y)
				y_max = vertex.y;
			if (z_max < vertex.z)
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

glm::vec3 Model::aabb()
{
	glm::vec3 aabb_scale(0);
	for (unsigned int i = 0; i < out_vertices.size(); i++)
	{
		aabb_scale = glm::vec3(glm::abs(Max_x - Min_x) * 0.5f,
			glm::abs(Max_y - Min_y) * 0.5f,
			glm::abs(Max_z - Min_z) * 0.5f);
	}
	return aabb_scale;
}

glm::vec3 Model::GetCentroid()
{
	float distance = 0, max_distance = 0;
	glm::vec3 centroid_scale;

	for (unsigned int i = 0; i < out_vertices.size(); i++)
	{
		distance = glm::length(out_vertices[i]);

		if (distance > max_distance)
		{
			max_distance = distance;
			centroid_scale = glm::vec3(max_distance, max_distance, max_distance);
		}
	}
	return centroid_scale;
}

float Model::GetRitter()
{
	float xmax_=0, xmin_=0, ymax_=0, ymin_=0, zmax_=0, zmin_=0;
	glm::vec3 xmaxVert, xminVert, ymaxVert, yminVert, zmaxVert, zminVert, minVertices, maxVertices;
	for (unsigned int i = 0; i < out_vertices.size(); i++)
	{
		if (xmax_ < out_vertices[i].x)
		{
			xmax_ = out_vertices[i].x;
			xmaxVert = out_vertices[i];
		}
		if (xmin_ > out_vertices[i].x)
		{
			xmin_ = out_vertices[i].x;
			xminVert = out_vertices[i];
		}
		if (ymax_ < out_vertices[i].y)
		{
			ymax_ = out_vertices[i].y;
			ymaxVert = out_vertices[i];
		}
		if (ymin_ > out_vertices[i].y)
		{
			ymin_ = out_vertices[i].y;
			yminVert = out_vertices[i];
		}
		if (zmax_ < out_vertices[i].z)
		{
			zmax_ = out_vertices[i].z;
			zmaxVert = out_vertices[i];
		}
		if (zmin_ > out_vertices[i].z)
		{
			zmin_ = out_vertices[i].z;
			zminVert = out_vertices[i];
		}
	}

	if (glm::abs(xmax_ - xmin_) > glm::abs(ymax_ - ymin_) &&
		glm::abs(xmax_ - xmin_) > glm::abs(zmax_ - zmin_))
	{
		maxVertices = xmaxVert;
		minVertices = xminVert;
	}
	else if (glm::abs(ymax_ - ymin_) > glm::abs(xmax_ - xmin_) &&
			glm::abs(ymax_ - ymin_) > glm::abs(zmax_ - zmin_))
	{
		maxVertices = ymaxVert;
		minVertices = yminVert;
	}
	else if (glm::abs(zmax_ - zmin_) > glm::abs(xmax_ - xmin_) &&
			glm::abs(zmax_ - zmin_) > glm::abs(ymax_ - ymin_))
	{
		maxVertices = zmaxVert;
		minVertices = zminVert;
	}
	
	ritter_center = glm::vec3(minVertices + maxVertices)*0.5f;			
	ritter_radius = glm::distance(maxVertices , minVertices)*0.5f;

	for (unsigned int j = 0; j < out_vertices.size(); j++)
	{
		float new_radius = glm::distance(ritter_center , out_vertices[j]);
		glm::vec3 dir_vec = glm::normalize(out_vertices[j] - ritter_center);

		if (ritter_radius < new_radius)
		{
			ritter_center = ((ritter_center - dir_vec * ritter_radius) + out_vertices[j])*0.5f;
			ritter_radius = glm::distance(ritter_center , out_vertices[j]);
		}
	}

	return ritter_radius;
}

float Model::GetEOPS()
{
	int xmin = 0, xmax = 0, ymin = 0, ymax = 0, zmin = 0, zmax = 0;
	glm::vec3 x_vec(1, 0, 0), y_vec(0, 1, 0), z_vec(0, 0, 1);
	glm::vec3 xminPt(0), xmaxPt(0), yminPt(0), ymaxPt(0), zminPt(0), zmaxPt(0);
	glm::vec3 min_pt(0), max_pt(0);
	glm::vec3 eops_mid(0);
	float eops_radius = 0;

	for (unsigned int i = 0; i < out_vertices.size(); i++)
	{
		ExtremePointsAlongDirection(x_vec, out_vertices, out_vertices.size(), &xmin, &xmax);
		ExtremePointsAlongDirection(y_vec, out_vertices, out_vertices.size(), &ymin, &ymax);
		ExtremePointsAlongDirection(z_vec, out_vertices, out_vertices.size(), &zmin, &zmax);

		xminPt = out_vertices[xmin];
		xmaxPt = out_vertices[xmax];
		yminPt = out_vertices[ymin];
		ymaxPt = out_vertices[ymax];
		zminPt = out_vertices[zmin];
		zmaxPt = out_vertices[zmax];

		float x_distance = glm::sqrt(glm::dot(x_maxpt - x_minpt, x_maxpt - x_minpt));
		float y_distance = glm::sqrt(glm::dot(y_maxpt - y_minpt, y_maxpt - y_minpt));
		float z_distance = glm::sqrt(glm::dot(z_maxpt - z_minpt, z_maxpt - z_minpt));

		float max_distance = 0;

		if (x_distance > y_distance && x_distance > z_distance)
		{
			max_distance = x_distance;
			max_pt = xmaxPt;
			min_pt = xminPt;
		}
		if (y_distance > x_distance && y_distance > z_distance)
		{
			max_distance = y_distance;
			max_pt = ymaxPt;
			min_pt = yminPt;
		}
		if (z_distance > y_distance && z_distance > x_distance)
		{
			max_distance = z_distance;
			max_pt = zmaxPt;
			min_pt = zminPt;
		}

		eops_mid = (max_pt + min_pt)*0.5f;
		eops_radius = max_distance * 0.5f;	
	}

	for (unsigned int j = 0; j < out_vertices.size(); j++)
	{
		float new_radius = glm::distance(eops_mid, out_vertices[j]);
		glm::vec3 dir_vec = glm::normalize(out_vertices[j] - eops_mid);

		if (eops_radius < new_radius)
		{
			eops_mid = ((eops_mid - (dir_vec * eops_radius)) + out_vertices[j])*0.5f;
			eops_radius = glm::distance(eops_mid, out_vertices[j]);
		}
	}
	return eops_radius;
}

glm::mat4 Model::SetRotation(glm::mat3& mat)
{
	glm::mat4 res;
	res[0] = glm::vec4(mat[0], 0);
	res[1] = glm::vec4(mat[1], 0);
	res[2] = glm::vec4(mat[2], 0);
	res[3] = glm::vec4(0, 0, 0, 1);

	return res;
}

float Model::Variance(float x[], int n)
{
	float u = 0.0f;
	for (int i = 0; i < n; i++)
		u += x[i];
	u /= n;
	float s2 = 0.0f;
	for (int i = 0; i < n; i++)
		s2 += (x[i] - u) * (x[i] - u);
	return s2 / n;
	return 0.0f;
}

void Model::CovarianceMatrix(glm::mat3x3 &cov, std::vector<glm::vec3>& vert, int numPts)
{
	float oon = 1.0f / (float)numPts;
	glm::vec3 c = glm::vec3(0.0f, 0.0f, 0.0f);
	float e00, e11, e22, e01, e02, e12;

	for (int i = 0; i < numPts; i++)
		c += vert[i];
	c *= oon;

	e00 = e11 = e22 = e01 = e02 = e12 = 0.0f;
	for (int i = 0; i < numPts; i++) {

		glm::vec3 p = vert[i] - c;
		
		e00 += p.x * p.x;
		e11 += p.y * p.y;
		e22 += p.z * p.z;
		e01 += p.x * p.y;
		e02 += p.x * p.z;
		e12 += p.y * p.z;
	}

	cov[0][0] = e00 * oon;
	cov[1][1] = e11 * oon;
	cov[2][2] = e22 * oon;
	cov[0][1] = cov[1][0] = e01 * oon;
	cov[0][2] = cov[2][0] = e02 * oon;
	cov[1][2] = cov[2][1] = e12 * oon;
}


void Model::SymSchur2(glm::mat3x3 &a, int p, int q, float &c, float &s)
{
	if (glm::abs(a[p][q]) > 0.0001f) {
		float r = (a[q][q] - a[p][p]) / (2.0f * a[p][q]);
		float t;
		if (r >= 0.0f)
			t = 1.0f / (r + glm::sqrt(1.0f + r * r));
		else
			t = -1.0f / (-r + glm::sqrt(1.0f + r * r));
		c = 1.0f / glm::sqrt(1.0f + t * t);
		s = t * c;
	}
	else {
		c = 1.0f;
		s = 0.0f;
	}
}

void Model::Jacobi(glm::mat3x3 &a, glm::mat3x3 &v)
{
	int i, j, n, p, q;
	float prevoff, c, s;
	glm::mat3x3 J;

	for (i = 0; i < 3; i++) {
		v[i][0] = v[i][1] = v[i][2] = 0.0f;
		v[i][i] = 1.0f;
	}

	const int MAX_ITERATIONS = 50;
	for (n = 0; n < MAX_ITERATIONS; n++) {

		p = 0; q = 1;
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				if (i == j) continue;
				if (glm::abs(a[i][j]) > glm::abs(a[p][q])) {
					p = i;
					q = j;
				}
			}
		}

		SymSchur2(a, p, q, c, s);
		for (i = 0; i < 3; i++) {
			J[i][0] = J[i][1] = J[i][2] = 0.0f;
			J[i][i] = 1.0f;
		}
		J[p][p] = c; J[p][q] = s;
		J[q][p] = -s; J[q][q] = c;

		v = v * J;
	
		a = (glm::transpose(J) * a) * J;

		float off = 0.0f;
			for (i = 0; i < 3; i++) {
				for (j = 0; j < 3; j++) {
					if (i == j) continue;
					off += a[i][j] * a[i][j];
				}
			}

		if (n > 2 && off >= prevoff)
			return;
		prevoff = off;
	}
}

float Model::EigenSphere()
{
	glm::mat3x3 m, v;

	CovarianceMatrix(m, out_vertices, out_vertices.size());
	Jacobi(m, v);

	glm::vec3 e;
	int maxc = 0;
	float maxf, maxe = glm::abs(m[0][0]);
	if ((maxf = glm::abs(m[1][1])) > maxe) maxc = 1, maxe = maxf;
	if ((maxf = glm::abs(m[2][2])) > maxe) maxc = 2, maxe = maxf;

	e[0] = v[0][maxc];
	e[1] = v[1][maxc];
	e[2] = v[2][maxc];

	int imin, imax;
	ExtremePointsAlongDirection(e, out_vertices, out_vertices.size(), &imin, &imax);
	glm::vec3 minpt = out_vertices[imin];
	glm::vec3 maxpt = out_vertices[imax];
	float dist = glm::sqrt(glm::dot(maxpt - minpt, maxpt - minpt));

	pca_radius = dist * 0.5f;
	pca_mid = (minpt + maxpt) * 0.5f;

	for (unsigned int j = 0; j < out_vertices.size(); j++)
	{
		float new_radius = glm::distance(pca_mid, out_vertices[j]);
		glm::vec3 dir_vec = glm::normalize(out_vertices[j] - pca_mid);

		if (pca_radius < new_radius)
		{
			pca_mid = ((pca_mid - dir_vec * pca_radius) + out_vertices[j])*0.5f;
			pca_radius = glm::distance(pca_mid, out_vertices[j]);
		}
	}

	return pca_radius;
}

glm::vec3 Model::Ellipsoid()
{
	CovarianceMatrix(m_m, out_vertices, out_vertices.size());
	Jacobi(m_m, m_v);
	
	glm::vec3 ex,ey,ez;
	glm::vec3 res;
	int maxc = 0;
	float maxf, maxe = glm::abs(m_m[0][0]);
	if ((maxf = glm::abs(m_m[1][1])) > maxe) maxc = 1, maxe = maxf;
	if ((maxf = glm::abs(m_m[2][2])) > maxe) maxc = 2, maxe = maxf;

	ex[0] = m_v[0][0];
	ex[1] = m_v[1][0];
	ex[2] = m_v[2][0];

	ey[0] = m_v[0][1];
	ey[1] = m_v[1][1];
	ey[2] = m_v[2][1];

	ez[0] = m_v[0][2];
	ez[1] = m_v[1][2];
	ez[2] = m_v[2][2];

	int xmin=0, xmax=0, ymin=0, ymax=0, zmin=0, zmax=0;

	ExtremePointsAlongDirection(ex, out_vertices, out_vertices.size(), &xmin, &xmax);
	ExtremePointsAlongDirection(ey, out_vertices, out_vertices.size(), &ymin, &ymax);
	ExtremePointsAlongDirection(ez, out_vertices, out_vertices.size(), &zmin, &zmax);
	x_minpt = out_vertices[xmin];
	x_maxpt = out_vertices[xmax];
	y_minpt = out_vertices[ymin];
	y_maxpt = out_vertices[ymax];
	z_minpt = out_vertices[zmin];
	z_maxpt = out_vertices[zmax];

	float x_distance = glm::sqrt(glm::dot(x_maxpt - x_minpt, x_maxpt - x_minpt));
	float y_distance = glm::sqrt(glm::dot(y_maxpt - y_minpt, y_maxpt - y_minpt));
	float z_distance = glm::sqrt(glm::dot(z_maxpt - z_minpt, z_maxpt - z_minpt));

	res = glm::vec3(x_distance, y_distance, z_distance)*0.5f;

	return res;
}

void Model::ExtremePointsAlongDirection(glm::vec3& dir, std::vector<glm::vec3>& vert, int n, int *imin, int *imax)
{
	float minproj = FLT_MAX, maxproj = -FLT_MAX;
	for (int i = 0; i < n; i++)
	{
		float proj = glm::dot(vert[i], dir);
		if (proj < minproj) {
			minproj = proj;
			*imin = i;
		}
		if (proj > maxproj) {
			maxproj = proj;
			*imax = i;
		}
	}
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


//function for render quad
void Model::renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {

			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
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




