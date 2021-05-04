/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Model.h
Purpose: Defining model functions
Language: c, c++
Platform: visual studio 2017, window
Project: chiho.kwon_CS350_2
Author: <name : Chiho Kwon, student login : chiho.kwon ,student id : 180003018
Creation date: 2/24/2019
End Header --------------------------------------------------------*/

#pragma once
#include "glm/glm.hpp"
#include "Triangle.h"
#include <vector>
#include <algorithm>
#include <map>

class Camera;
class Shader;


class Model
{
	unsigned vao, vbo, ebo, tbo, normalbuffer;
	unsigned o_vao, o_vbo, o_ebo, o_tbo, color_buffer;
	unsigned b_vao, b_vbo, b_ebo, bsp_buffer;
	unsigned element_size;
	unsigned width, height;
	unsigned tex1, tex2;
	bool isUseTex;
	bool isUseBV;

	unsigned int quadVAO = 0;
	unsigned int quadVBO;

public:

	Model();
	~Model();

	int ProjectionType;
	int NormalCalculation;

	void Render(Camera* camera, Shader* shader, float aspect, glm::vec3 m_position, bool isUseBV, bool isUseRotation);
	void RenderPartitionColor(Camera* camera, Shader* shader, float aspect, glm::vec3 m_position);
	void RenderBspColor(Camera* camera, Shader* shader, float aspect, glm::vec3 m_position);
	void Draw(std::vector<glm::vec3>& vert, std::vector<unsigned>& indices, std::vector<glm::vec2> textures);
	void DrawOctree(std::vector<glm::vec3>& vert, std::vector<unsigned>& indices, std::vector<glm::vec2> textures);
	void DrawBsptree(std::vector<glm::vec3>& vert, std::vector<unsigned>& indices, std::vector<glm::vec2> textures);
	float x_max = 0, x_min = 0, y_max = 0, y_min = 0, z_max = 0, z_min = 0, max_abs = 0;
	float Max_x, Min_x, Max_y, Min_y, Max_z, Min_z;

	bool LoadObj(const char* path, bool isUseTex, bool isUseBV, glm::vec3 midpoint);

	glm::vec3 position, scale, color;

	std::vector<unsigned> vertex_indices;
	std::vector<glm::vec3> out_vertices;
	std::vector<Triangle> m_triangle;

	std::multimap<int, glm::vec3> faceNormal;
	std::multimap<int, glm::vec3> vertexNormal;
	std::vector<glm::vec3> normalVector;
	std::vector<glm::vec3> octree_color;
	std::vector<glm::vec3> bsptree_color;

	std::vector<glm::vec2> textureUV;
	std::vector<glm::vec3> TextSpecInfo;
	std::vector<glm::vec3> TextDiffInfo;
	int poly;
	void renderQuad();

	bool loadPPM(const char* path, std::vector<glm::vec3>& values_);
	void TextureData(Shader* shader);
	void TextureProcess();

	//Bounding voulmes
	float rotation;

	//for aabb
	glm::vec3 aabb();

	//for centroid
	glm::vec3 GetCentroid();

	//for ritter method
	float ritter_radius;
	glm::vec3 ritter_center;
	float GetRitter();

	//for pca sphere
	float pca_radius = 0;
	glm::vec3 pca_mid;

	//for ritter method
	glm::vec3 eops_center;
	float GetEOPS();

	//for ellipse
	glm::vec3 x_minpt, x_maxpt, y_minpt, y_maxpt, z_minpt, z_maxpt;
	std::vector<glm::vec3> modelPos_vec;
	bool isUseRotation;
	glm::mat3x3 m_m, m_v;
	glm::mat4 SetRotation(glm::mat3& mat);

	
	float Variance(float x[], int n);
	void CovarianceMatrix(glm::mat3x3 &cov, std::vector<glm::vec3>& vert, int numPts);
	void SymSchur2(glm::mat3x3 &a, int p, int q, float &c, float &s);
	void Jacobi(glm::mat3x3 &a, glm::mat3x3 &v);
	float EigenSphere();
	glm::vec3 Ellipsoid();
	void ExtremePointsAlongDirection(glm::vec3& dir, std::vector<glm::vec3>& vert, int n, int *imin, int *imax);
};