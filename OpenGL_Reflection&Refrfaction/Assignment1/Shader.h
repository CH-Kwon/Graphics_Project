/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.h
Purpose: Defining shader function
Language: c, c++
Platform: visual studio 2017, window
Project: chiho.kwon_CS300_3
Author: <name : Chiho Kwon, student login : chiho.kwon ,student id : 180003018
Creation date: 11/08/2018
End Header --------------------------------------------------------*/

#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <string>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <sstream>

class Shader {

public:

	enum Type {NORMAL, PHONG_SHADING, CUBEMAP,SKYBOX};
	//enum Light {NO_LIGHT,POINT_LIGHT, DIRECTION_LIGHT, SPOT_LIGHT};

	void CreateShader(const char* vert_path, const char* frag_path, const char* geo_path);
	void Use() const;

	void SetBool(const std::string &name, bool value) const;
	void SetuInt(const std::string &name, unsigned value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
	void SetVec3(const std::string &name, const glm::vec3 &value) const;
	void SetVec3(const std::string &name, float x, float y, float z) const;
	void SetVec4(const std::string &name, const glm::vec4 &value) const;
	void SetVec4(const std::string &name, float x, float y, float z, float w) const;
	void SetMat4(const std::string &name, const glm::mat4 &mat) const;
	Shader(bool isGeoUse, Type type);
	~Shader();

	GLuint m_programId, m_vertexId, m_fragmentId, m_geometryId, m_bli;
	GLint m_infoLogLength = 0;
	GLint m_result;

	bool isGeoUse;

	Type m_norm;
	//Light m_light;
	
};
