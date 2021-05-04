/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.cpp
Purpose: For using shader functions
Language: c, c++
Platform: visual studio 2017, window
Project: chiho.kwon_CS350_2
Author: <name : Chiho Kwon, student login : chiho.kwon ,student id : 180003018
Creation date: 2/24/2019
End Header --------------------------------------------------------*/

#include <vector>
#include "Shader.h"

#include "glad/glad.h"

Shader::Shader(bool GeoUse, Type type)
	:m_programId(0), m_vertexId(0), m_fragmentId(0), m_geometryId(0),
	m_infoLogLength(0), m_result(GL_FALSE), isGeoUse(GeoUse), m_norm(type)
{
	const char* Unreferenced = "";
	if (type == NORMAL)
	{
		CreateShader("shaders\\m_vertexShader.vert", "shaders\\m_fragmentShader.frag", Unreferenced);
	}
	if(GeoUse == true)
	{
		if (type == VERTEX)
		{
			CreateShader("shaders\\m_vertexShader_2.vert", "shaders\\m_fragmentShader_2.frag", "shaders\\m_geometryShader.geom");
		}
		else if (type == FACE)
		{
			CreateShader("shaders\\m_vertexShader_2.vert", "shaders\\m_fragmentShader_2.frag", "shaders\\m_geometryShader_2.geom");
		}
	}
	if(type == DEFFERED)
	{
		CreateShader("shaders\\deferred_shading.vert", "shaders\\deferred_shading.frag", Unreferenced);
	}
	if (type == G_BUFFER)
	{
		CreateShader("shaders\\g_buffer.vert", "shaders\\g_buffer.frag", Unreferenced);
	}
	if (type == OCTREE)
	{
		CreateShader("shaders\\m_octree.vert", "shaders\\m_octree.frag", Unreferenced);
	}
}

Shader::~Shader() {}

void Shader::CreateShader(const char* vert_path, const char* frag_path, const char* geo_path)
{
	std::string VertShaderCode, FragShaderCode, GeoShaderCode;
	std::ifstream VertexShadereStream(vert_path, std::ios::in);
	std::ifstream FragmentShaderStream(frag_path, std::ios::in);
	std::ifstream GeometryShaderStream(geo_path, std::ios::in);
	if (VertexShadereStream.is_open())
	{
		std::stringstream vShaderStream;
		vShaderStream << VertexShadereStream.rdbuf();
		VertexShadereStream.close();
		VertShaderCode = vShaderStream.str();
	}
	if (FragmentShaderStream.is_open())
	{
		std::stringstream fShaderStream;
		fShaderStream << FragmentShaderStream.rdbuf();
		FragmentShaderStream.close();
		FragShaderCode = fShaderStream.str();
	}
	if (GeometryShaderStream.is_open())
	{
		std::stringstream gShaderStream;
		gShaderStream << GeometryShaderStream.rdbuf();
		GeometryShaderStream.close();
		GeoShaderCode = gShaderStream.str();
	}
	const char* vShaderCode = VertShaderCode.c_str();
	const char* fShaderCode = FragShaderCode.c_str();
	const char* gShaderCode = GeoShaderCode.c_str();

	m_vertexId = glCreateShader(GL_VERTEX_SHADER);
	char const* vertexSourcePointer = VertShaderCode.c_str();
	glShaderSource(m_vertexId, 1, &vertexSourcePointer, NULL);
	glCompileShader(m_vertexId);
	glGetShaderiv(m_vertexId, GL_COMPILE_STATUS, &m_result);
	if (m_result == GL_FALSE)
	{
		glGetShaderiv(m_vertexId, GL_INFO_LOG_LENGTH, &m_infoLogLength);
		std::vector<char> ShaderErrorMessage(m_infoLogLength + 1);
		glGetShaderInfoLog(m_vertexId, 1024, &m_infoLogLength, &ShaderErrorMessage[0]);
		printf("Shader - %4s\n vertex problem", &ShaderErrorMessage[0]);
	}

	m_fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
	char const* fragmentSourcePointer = FragShaderCode.c_str();
	glShaderSource(m_fragmentId, 1, &fragmentSourcePointer, NULL);
	glCompileShader(m_fragmentId);
	glGetShaderiv(m_fragmentId, GL_COMPILE_STATUS, &m_result);
	if (m_result == GL_FALSE) {

		glGetShaderiv(m_fragmentId, GL_INFO_LOG_LENGTH, &m_infoLogLength);
		std::vector<char> ShaderErrorMessage(m_infoLogLength + 1);
		glGetShaderInfoLog(m_fragmentId, 1024, &m_infoLogLength, &ShaderErrorMessage[0]);
		printf("Shader - %4s\n frag problem", &ShaderErrorMessage[0]);
	}

	if (isGeoUse == true)
	{
		m_geometryId = glCreateShader(GL_GEOMETRY_SHADER);
		char const* geometrySourcePointer = GeoShaderCode.c_str();
		glShaderSource(m_geometryId, 1, &geometrySourcePointer, NULL);
		glCompileShader(m_geometryId);
		glGetShaderiv(m_geometryId, GL_COMPILE_STATUS, &m_result);
		if (m_result == GL_FALSE) {

			glGetShaderiv(m_result, GL_INFO_LOG_LENGTH, &m_infoLogLength);
			std::vector<char> ShaderErrorMessage(m_infoLogLength + 1);
			glGetShaderInfoLog(m_result, 1024, &m_infoLogLength, &ShaderErrorMessage[0]);
			printf("Shader - %4s\n geo problem", &ShaderErrorMessage[0]);
		}
	}

	m_programId = glCreateProgram();

	if (m_programId == 0)
	{
		printf("Shader - Shader couldn't get program id.\n");
	}
	else 
	{
		glAttachShader(m_programId, m_vertexId);
		glAttachShader(m_programId, m_fragmentId);
		if (isGeoUse == true)
		{
			glAttachShader(m_programId, m_geometryId);
		}
		glLinkProgram(m_programId);

		glGetProgramiv(m_programId, GL_LINK_STATUS, &m_result);

		if (m_infoLogLength > 0) {
			glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &m_infoLogLength);
			std::vector<char> ProgramErrorMessage(m_infoLogLength + 1);
			glGetProgramInfoLog(m_programId, 1024, &m_infoLogLength, &ProgramErrorMessage[0]);
			printf("Shader: %4s\n wrong", &ProgramErrorMessage[0]);
		}
		glDeleteShader(m_vertexId);
		glDeleteShader(m_fragmentId);
		if (isGeoUse == true)
		{
			glDeleteShader(m_geometryId);
		}
	}
}

void Shader::Use() const
{
	glUseProgram(m_programId);
}

void Shader::SetBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_programId, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
}

void Shader::SetuInt(const std::string &name, unsigned value) const
{
	glUniform1ui(glGetUniformLocation(m_programId, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string &name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(m_programId, name.c_str()), x, y, z, w);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

