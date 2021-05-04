#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <math.h>
#include "Model.h"
#include "Camera.h"
#include "Shader.h"

struct OctreeNode 
{
	OctreeNode(){}
	glm::vec3 octree_center; // Center point of octree node (not strictly needed)
	float octree_width; // Half the width of the node volume (not strictly needed)
	OctreeNode *pChild[8]; // Pointers to the eight children nodes
	Model *pObjList; // Linked list of objects contained at this node
	int octree_depth;
	void DrawBox(Model* mod, Camera* camera, Shader* shader,
		float aspect, bool isUseBV, bool isUseRotation, int stopdepth);
};

struct Object 
{
	glm::vec3 center; // Center point for object
	float radius; // Radius of object bounding sphere

	Model *pNextObject; // Pointer to next object when linked into list
};


	OctreeNode* BuildOctree(std::vector<Model*> mod, glm::vec3 center, float halfWidth, int stopdepth);
	int GetTriNum(std::vector<Model*> mod, glm::vec3& center, float& halfWidth, glm::vec3 color);
	glm::vec3 GetCenter(std::vector<Model*> mod);
	float GetLongest(std::vector<Model*> mod);

	//float mod_xMax = -FLT_MAX, mod_xMin = FLT_MAX, mod_yMax = -FLT_MAX, mod_yMin = FLT_MAX, mod_zMin = FLT_MAX, mod_zMax = -FLT_MAX;
	//float x_dist = 0, y_dist = 0, z_dist = 0;
	//float dist = 0;


