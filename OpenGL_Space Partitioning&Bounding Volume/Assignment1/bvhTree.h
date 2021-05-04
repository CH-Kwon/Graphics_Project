#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <math.h>
#include "Model.h"
#include "Camera.h"
#include "Shader.h"

enum TreeType {NODE, LEAF};

struct Node
{
	struct Node *pRoot;

	struct Node *pLeft = 0;
	struct Node *pRight = 0;

	TreeType tree_type;

	int numOfObjects;
	int numOfVertices;
	std::vector<Model> objects;
	int tree_depth;//for balnace

	glm::vec3 bv_scale;
	glm::vec3 bv_scale_circle;
	glm::vec3 bv_mid;
	void DrawBox(int depth, Model* mod, Camera* camera, Shader* shader,
		float aspect, bool isUseBV, bool isUseRotation, int drawType);
};

class Tree
{
public:

	void ComputeBoundingVolume(std::vector<glm::vec3>& obj, glm::vec3& mid, glm::vec3& scale, glm::vec3& scale_circle);
	void PartitionObjects(std::vector<glm::vec3>& obj, std::vector<glm::vec3>& left, std::vector<glm::vec3>& right, glm::vec3 &middle);
	void TopDownBVTree(Node **tree, std::vector<glm::vec3>& obj, int depth);

	float model_xMax = -FLT_MAX, model_xMin = FLT_MAX, model_yMax = -FLT_MAX, model_yMin = FLT_MAX, model_zMin = FLT_MAX, model_zMax = -FLT_MAX;
	float dist_x = 0, dist_y = 0, dist_z = 0;

	float modelVert_xmax = 0, modelVert_xmin = 0, modelVert_ymax = 0, modelVert_ymin = 0,
		modelVert_zmax = 0, modelVert_zmin = 0;

	float longest_dist = 0;
	float xMid = 0, yMid = 0, zMid = 0;

};