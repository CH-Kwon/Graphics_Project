/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: BVH.cpp
Purpose: For calculating BVH
Language: c, c++
Platform: visual studio 2017, window
Project: chiho.kwon_CS350_2
Author: <name : Chiho Kwon, student login : chiho.kwon ,student id : 180003018
Creation date: 2/24/2019
End Header --------------------------------------------------------*/

#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <algorithm>
#include <map>

struct Node
{
	Node *left;
	Node *right;
};

class BVH
{

public:

	BVH();
	~BVH();

private:
	Node *root;
};