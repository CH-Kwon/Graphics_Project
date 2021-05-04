#include "bspTree.h"

#define MAX_DEPTH 8
#define MIN_LEAF_SIZE 300

BSPNode *BuildBSPTree(std::vector<Triangle>& polygons, std::vector<Model*>& mod, int depth,
	std::vector<int>& obj_index, std::vector<std::vector<unsigned>>& obj_indices)
{
	float R_ = (rand() % 100) / 100.f;
	float G_ = (rand() % 100) / 100.f;
	float B_ = (rand() % 100) / 100.f;
	glm::vec3 color(R_, G_, B_);
	
	if (polygons.empty()) return NULL;

	int numPolygons = polygons.size();
	
	if (depth <= 0 || numPolygons <= MIN_LEAF_SIZE)
	return new BSPNode(polygons);

	BSPNode* pNode = new BSPNode(polygons);

	Plane splitPlane = PickSplittingPlane(polygons);
	std::vector<Triangle> frontList, backList;
	std::vector<int> front_obj_index, back_obj_index;
	std::vector<std::vector<unsigned>> front_obj_indices, back_obj_indices;

	for (int i = 0; i < numPolygons; i++) 
	{
		mod[obj_index[i]]->bsptree_color[obj_indices[i][0]] = color;
		mod[obj_index[i]]->bsptree_color[obj_indices[i][1]] = color;
		mod[obj_index[i]]->bsptree_color[obj_indices[i][2]] = color;
		Triangle poly = polygons[i];
		switch (ClassifyPolygonToPlane(poly, splitPlane))
		{
		case COPLANAR_WITH_PLANE:
			break;
		case IN_FRONT_OF_PLANE:
			frontList.push_back(poly);
			front_obj_index.push_back(obj_index[i]);
			front_obj_indices.push_back(obj_indices[i]);
			break;
		case BEHIND_PLANE:
			backList.push_back(poly);
			back_obj_index.push_back(obj_index[i]);
			back_obj_indices.push_back(obj_indices[i]);
			break;
		case STRADDLING_PLANE:
			frontList.push_back(poly);
			backList.push_back(poly);
			front_obj_index.push_back(obj_index[i]);
			back_obj_index.push_back(obj_index[i]);
			front_obj_indices.push_back(obj_indices[i]);
			back_obj_indices.push_back(obj_indices[i]);
			break;
		}
	}
	
	pNode->m_front = BuildBSPTree(frontList, mod, depth - 1, front_obj_index, front_obj_indices);
	pNode->m_back = BuildBSPTree(backList, mod, depth - 1, back_obj_index, back_obj_indices);
	return pNode;
}

Plane PickSplittingPlane(std::vector<Triangle> &polygons)
{
	const float K = 0.8f;

	Plane bestPlane;
	float bestScore = FLT_MAX;

	for (unsigned int i = 0; i < polygons.size(); i++) {
		int numInFront = 0, numBehind = 0, numStraddling = 0;
		Plane plane = GetPlaneFromPolygon(polygons[i]);
	
		for (unsigned int j = 0; j < polygons.size(); j++) 
		{
			if (i == j) continue;
		
			switch (ClassifyPolygonToPlane(polygons[j], plane)) 
			{
			case COPLANAR_WITH_PLANE:
				break;
			case IN_FRONT_OF_PLANE:
				numInFront++;
				break;
			case BEHIND_PLANE:
				numBehind++;
				break;
			case STRADDLING_PLANE:
				numStraddling++;
				break;
			}
		}
	
		float score = K * numStraddling + (1.0f - K) * glm::abs(numInFront - numBehind);
		if (score < bestScore) {
			bestScore = score;
			bestPlane = plane;
		}
	}
	return bestPlane;
}

int ClassifyPointToPlane(glm::vec3 p, Plane plane)
{
	float dist = glm::dot(plane.n, p) - plane.d;
	float epsilon = std::numeric_limits<float>::epsilon();
	
	if (dist > epsilon)
		return POINT_IN_FRONT_OF_PLANE;
	if (dist < -epsilon)
		return POINT_BEHIND_PLANE;
	return POINT_ON_PLANE;
}

int ClassifyPolygonToPlane(Triangle poly, Plane plane)
{
	int numInFront = 0, numBehind = 0;
	int numVerts = 3;
	for (int i = 0; i < numVerts; i++) 
	{
		glm::vec3 p = poly.pt[i];
		switch (ClassifyPointToPlane(p, plane))
		{
		case POINT_IN_FRONT_OF_PLANE:
			numInFront++;
			break;
		case POINT_BEHIND_PLANE:
			numBehind++;
			break;
		}
	}
	
	if (numBehind != 0 && numInFront != 0)
		return STRADDLING_PLANE;

	if (numInFront != 0)
		return IN_FRONT_OF_PLANE;

	if (numBehind != 0)
		return BEHIND_PLANE;
	
	return COPLANAR_WITH_PLANE;
}

Plane GetPlaneFromPolygon(Triangle poly)
{
	Plane p;
	p.n = glm::normalize(glm::cross(poly.pt[1] - poly.pt[0], poly.pt[2] - poly.pt[0]));
	p.d = glm::dot(p.n, poly.pt[0]);
	return p;
}


