/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Light.h
Purpose: For using lighting
Language: c, c++
Platform: visual studio 2017, window
Project: chiho.kwon_CS300_3
Author: <name : Chiho Kwon, student login : chiho.kwon ,student id : 180003018
Creation date: 11/08/2018
End Header --------------------------------------------------------*/

#pragma once
#include "glm/glm.hpp"
#include "Shader.h"

class Light
{
public:

	Light() {};
	~Light() {}

	glm::vec3 color; 
	glm::vec3 position[16] = {			 glm::vec3(0.0f,0.0f,0.0f), //sphere1
										 glm::vec3(0.0f,0.0f,0.0f), //sphere2
										 glm::vec3(0.0f,0.0f,0.0f), //sphere3
										 glm::vec3(0.0f,0.0f,0.0f), //sphere4
										 glm::vec3(0.0f,0.0f,0.0f), //sphere5
										 glm::vec3(0.0f,0.0f,0.0f), //sphere6
										 glm::vec3(0.0f,0.0f,0.0f), //sphere7
										 glm::vec3(0.0f,0.0f,0.0f), //sphere8
										 glm::vec3(0.0f,0.0f,0.0f), //sphere9
										 glm::vec3(0.0f,0.0f,0.0f), //sphere10
										 glm::vec3(0.0f,0.0f,0.0f), //sphere11
										 glm::vec3(0.0f,0.0f,0.0f), //sphere12
										 glm::vec3(0.0f,0.0f,0.0f), //sphere13
										 glm::vec3(0.0f,0.0f,0.0f), //sphere14
										 glm::vec3(0.0f,0.0f,0.0f), //sphere15
										 glm::vec3(0.0f,0.0f,0.0f), //sphere16
	};			


	int sphere_max = 2;
	
	bool isDir = false;
	bool isPoint = true;
	bool isSpot = false;

	glm::vec3 lightColor[16] =		 {   glm::vec3(0.1f,0.8f,0.1f), //sphere1
										 glm::vec3(0.8f,0.8f,0.1f), //sphere2
										 glm::vec3(0.0f,0.0f,0.7f), //sphere3
										 glm::vec3(0.7f,0.7f,0.0f), //sphere4
										 glm::vec3(0.0f,0.7f,0.7f), //sphere5
									  	 glm::vec3(0.7f,0.0f,0.7f), //sphere6
										 glm::vec3(0.3f,0.3f,0.8f), //sphere7
									 	 glm::vec3(0.3f,0.8f,0.3f), //sphere8
										 glm::vec3(0.8f,0.3f,0.3f), //sphere9
										 glm::vec3(0.5f,0.5f,0.5f), //sphere10
										 glm::vec3(0.2f,0.2f,0.2f), //sphere11
										 glm::vec3(0.1f,0.5f,0.7f), //sphere12
										 glm::vec3(0.7f,0.5f,0.1f), //sphere13
										 glm::vec3(0.9f,0.1f,0.9f), //sphere14
										 glm::vec3(0.2f,0.4f,0.8f), //sphere15
										 glm::vec3(0.8f,0.4f,0.2f), //sphere16
										};
	glm::vec3 lightColor2[16] =	 { glm::vec3(0.7f,0.0f,0.0f), //sphere1
										 glm::vec3(0.0f,0.7f,0.0f), //sphere2
										 glm::vec3(0.0f,0.0f,0.7f), //sphere3
										 glm::vec3(0.7f,0.7f,0.0f), //sphere4
										 glm::vec3(0.0f,0.7f,0.7f), //sphere5
										 glm::vec3(0.7f,0.0f,0.7f), //sphere6
										 glm::vec3(0.3f,0.3f,0.8f), //sphere7
										 glm::vec3(0.3f,0.8f,0.3f), //sphere8
										 glm::vec3(0.8f,0.3f,0.3f), //sphere9
										 glm::vec3(0.5f,0.5f,0.5f), //sphere10
										 glm::vec3(0.2f,0.2f,0.2f), //sphere11
										 glm::vec3(0.1f,0.5f,0.7f), //sphere12
										 glm::vec3(0.7f,0.5f,0.1f), //sphere13
										 glm::vec3(0.9f,0.1f,0.9f), //sphere14
										 glm::vec3(0.2f,0.4f,0.8f), //sphere15
										 glm::vec3(0.8f,0.4f,0.2f), //sphere16
	};

	glm::vec3 ambient[16] =     {		 glm::vec3(0.0f,0.0f,0.0f),	//sphere1
										 glm::vec3(0.0f,0.0f,0.0f),	//sphere2
										 glm::vec3(0.15f,0.15f,0.15f),	//sphere3
										 glm::vec3(0.15f,0.15f,0.15f),	//sphere4
										 glm::vec3(0.15f,0.15f,0.15f),	//sphere5
										 glm::vec3(0.15f,0.15f,0.15f),	//sphere6
										 glm::vec3(0.15f,0.15f,0.15f),	//sphere7
										 glm::vec3(0.15f,0.15f,0.15f),	//sphere8
										 glm::vec3(0.15f,0.15f,0.15f),	//sphere9
										 glm::vec3(0.15f,0.15f,0.15f),	//sphere10
										 glm::vec3(0.15f,0.15f,0.15f),	//sphere11
										 glm::vec3(0.15f,0.15f,0.15f),	//sphere12
										 glm::vec3(0.15f,0.15f,0.15f),	//sphere13
										 glm::vec3(0.15f,0.15f,0.15f),	//sphere14
										 glm::vec3(0.15f,0.15f,0.15f),	//sphere15
										 glm::vec3(0.15f,0.15f,0.15f)	//sphere16
							   };

	float inner_angle[16] =			 { 12.5f, 12.5f, 12.5f, 12.5f, 12.5f, 12.5f,
									   12.5f, 12.5f, 12.5f, 12.5f, 12.5f, 12.5f,
		                               12.5f, 12.5f, 12.5f, 12.5f
										};

	float outer_angle[16] = {
									25.f,25.f,25.f,25.f,25.f,25.f,
									25.f,25.f,25.f,25.f,25.f,25.f,
									25.f,25.f,25.f,25.f
	};

	float falloff[16] = { 10.f, 10.f, 10.f, 10.f, 10.f,
						 10.f, 10.f, 10.f, 10.f, 10.f,
						 10.f, 10.f, 10.f, 10.f, 10.f, 10.f
	};
	

	void Update(Shader* shader)
	{
		shader->SetVec3("FragColor", color);

		shader->SetBool("isDir", isDir);
		shader->SetBool("isPoint", isPoint);
		shader->SetBool("isSpot", isSpot);
		shader->SetInt("sphere_max", sphere_max);
		//Sphere1
		shader->SetVec3("lightColor[0]", lightColor[0]);
		shader->SetVec3("dirLight[0].position", position[0]);
		shader->SetVec3("dirLight[0].direction", -position[0]);
		shader->SetVec3("dirLight[0].ambient", ambient[0]);
		shader->SetVec3("dirLight[0].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("dirLight[0].specular", glm::vec3(0.2f, 0.2f, 0.2f));

		shader->SetVec3("pointLights[0].position", position[0]);
		shader->SetVec3("pointLights[0].ambient", ambient[0]);
		shader->SetVec3("pointLights[0].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		shader->SetVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("pointLights[0].constant", 1.0f);
		shader->SetFloat("pointLights[0].linear", 0.09f);
		shader->SetFloat("pointLights[0].quadratic", 0.032f);

		shader->SetVec3("spotLight[0].position", position[0]);
		shader->SetVec3("spotLight[0].direction", position[0]);
		shader->SetVec3("spotLight[0].ambient", ambient[0]);
		shader->SetVec3("spotLight[0].diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader->SetVec3("spotLight[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("spotLight[0].constant", 1.0f);
		shader->SetFloat("spotLight[0].linear", 0.09f);
		shader->SetFloat("spotLight[0].quadratic", 0.032f);
		shader->SetFloat("spotLight[0].cutOff", glm::cos(glm::radians(inner_angle[0])));
		shader->SetFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(outer_angle[0])));
		shader->SetFloat("spotLight[0].falloff", falloff[0]);

		//Sphere2
		shader->SetVec3("lightColor[1]", lightColor[1]);
		shader->SetVec3("dirLight[1].position", position[1]);
		shader->SetVec3("dirLight[1].direction", -position[1]);
		shader->SetVec3("dirLight[1].ambient", ambient[1]);
		shader->SetVec3("dirLight[1].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("dirLight[1].specular", glm::vec3(0.5f, 0.5f, 0.5f));

		shader->SetVec3("pointLights[1].position", position[1]);
		shader->SetVec3("pointLights[1].ambient", ambient[1]);
		shader->SetVec3("pointLights[1].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		shader->SetVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("pointLights[1].constant", 1.0f);
		shader->SetFloat("pointLights[1].linear", 0.09f);
		shader->SetFloat("pointLights[1].quadratic", 0.032f);

		shader->SetVec3("spotLight[1].position", position[1]);
		shader->SetVec3("spotLight[1].direction", position[1]);
		shader->SetVec3("spotLight[1].ambient", ambient[1]);
		shader->SetVec3("spotLight[1].diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader->SetVec3("spotLight[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("spotLight[1].constant", 1.0f);
		shader->SetFloat("spotLight[1].linear", 0.09f);
		shader->SetFloat("spotLight[1].quadratic", 0.032f);
		shader->SetFloat("spotLight[1].cutOff", glm::cos(glm::radians(inner_angle[1])));
		shader->SetFloat("spotLight[1].outerCutOff", glm::cos(glm::radians(outer_angle[1])));
		shader->SetFloat("spotLight[1].falloff", falloff[1]);

		//Sphere3
		shader->SetVec3("lightColor[2]", lightColor[2]);
		shader->SetVec3("dirLight[2].position", position[2]);
		shader->SetVec3("dirLight[2].direction", -position[2]);
		shader->SetVec3("dirLight[2].ambient", ambient[2]);
		shader->SetVec3("dirLight[2].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("dirLight[2].specular", glm::vec3(0.5f, 0.5f, 0.5f));

		shader->SetVec3("pointLights[2].position", position[2]);
		shader->SetVec3("pointLights[2].ambient", ambient[2]);
		shader->SetVec3("pointLights[2].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("pointLights[2].constant", 1.0f);
		shader->SetFloat("pointLights[2].linear", 0.09f);
		shader->SetFloat("pointLights[2].quadratic", 0.032f);

		shader->SetVec3("spotLight[2].position", position[2]);
		shader->SetVec3("spotLight[2].direction", position[2]);
		shader->SetVec3("spotLight[2].ambient", ambient[2]);
		shader->SetVec3("spotLight[2].diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader->SetVec3("spotLight[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("spotLight[2].constant", 1.0f);
		shader->SetFloat("spotLight[2].linear", 0.09f);
		shader->SetFloat("spotLight[2].quadratic", 0.032f);
		shader->SetFloat("spotLight[2].cutOff", glm::cos(glm::radians(inner_angle[2])));
		shader->SetFloat("spotLight[2].outerCutOff", glm::cos(glm::radians(outer_angle[2])));
		shader->SetFloat("spotLight[2].falloff", falloff[2]);

		//Sphere4
		shader->SetVec3("lightColor[3]", lightColor[3]);
		shader->SetVec3("dirLight[3].position", position[3]);
		shader->SetVec3("dirLight[3].direction", -position[3]);
		shader->SetVec3("dirLight[3].ambient", ambient[3]);
		shader->SetVec3("dirLight[3].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("dirLight[3].specular", glm::vec3(0.5f, 0.5f, 0.5f));

		shader->SetVec3("pointLights[3].position", position[3]);
		shader->SetVec3("pointLights[3].ambient", ambient[3]);
		shader->SetVec3("pointLights[3].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("pointLights[3].specular", glm::vec3(0.5f, 0.5f, 0.5f));
		shader->SetFloat("pointLights[3].constant", 1.0f);
		shader->SetFloat("pointLights[3].linear", 0.09f);
		shader->SetFloat("pointLights[3].quadratic", 0.032f);

		shader->SetVec3("spotLight[3].position", position[3]);
		shader->SetVec3("spotLight[3].direction", position[3]);
		shader->SetVec3("spotLight[3].ambient", ambient[3]);
		shader->SetVec3("spotLight[3].diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader->SetVec3("spotLight[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("spotLight[3].constant", 1.0f);
		shader->SetFloat("spotLight[3].linear", 0.09f);
		shader->SetFloat("spotLight[3].quadratic", 0.032f);
		shader->SetFloat("spotLight[3].cutOff", glm::cos(glm::radians(inner_angle[3])));
		shader->SetFloat("spotLight[3].outerCutOff", glm::cos(glm::radians(outer_angle[3])));
		shader->SetFloat("spotLight[3].falloff", falloff[3]);

		//Sphere5
		shader->SetVec3("lightColor[4]", lightColor[4]);
		shader->SetVec3("dirLight[4].position", position[4]);
		shader->SetVec3("dirLight[4].direction", -position[4]);
		shader->SetVec3("dirLight[4].ambient", ambient[4]);
		shader->SetVec3("dirLight[4].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("dirLight[4].specular", glm::vec3(0.5f, 0.5f, 0.5f));

		shader->SetVec3("pointLights[4].position", position[4]);
		shader->SetVec3("pointLights[4].ambient", ambient[4]);
		shader->SetVec3("pointLights[4].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("pointLights[4].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("pointLights[4].constant", 1.0f);
		shader->SetFloat("pointLights[4].linear", 0.09f);
		shader->SetFloat("pointLights[4].quadratic", 0.032f);

		shader->SetVec3("spotLight[4].position", position[4]);
		shader->SetVec3("spotLight[4].direction", position[4]);
		shader->SetVec3("spotLight[4].ambient", ambient[4]);
		shader->SetVec3("spotLight[4].diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader->SetVec3("spotLight[4].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("spotLight[4].constant", 1.0f);
		shader->SetFloat("spotLight[4].linear", 0.09f);
		shader->SetFloat("spotLight[4].quadratic", 0.032f);
		shader->SetFloat("spotLight[4].cutOff", glm::cos(glm::radians(inner_angle[4])));
		shader->SetFloat("spotLight[4].outerCutOff", glm::cos(glm::radians(outer_angle[4])));
		shader->SetFloat("spotLight[4].falloff", falloff[4]);


		//Sphere6
		shader->SetVec3("lightColor[5]", lightColor[5]);
		shader->SetVec3("dirLight[5].position", position[5]);
		shader->SetVec3("dirLight[5].direction", -position[5]);
		shader->SetVec3("dirLight[5].ambient", ambient[5]);
		shader->SetVec3("dirLight[5].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("dirLight[5].specular", glm::vec3(0.5f, 0.5f, 0.5f));

		shader->SetVec3("pointLights[5].position", position[5]);
		shader->SetVec3("pointLights[5].ambient", ambient[5]);
		shader->SetVec3("pointLights[5].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("pointLights[5].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("pointLights[5].constant", 1.0f);
		shader->SetFloat("pointLights[5].linear", 0.09f);
		shader->SetFloat("pointLights[5].quadratic", 0.032f);

		shader->SetVec3("spotLight[5].position", position[5]);
		shader->SetVec3("spotLight[5].direction", position[5]);
		shader->SetVec3("spotLight[5].ambient", ambient[5]);
		shader->SetVec3("spotLight[5].diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader->SetVec3("spotLight[5].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("spotLight[5].constant", 1.0f);
		shader->SetFloat("spotLight[5].linear", 0.09f);
		shader->SetFloat("spotLight[5].quadratic", 0.032f);
		shader->SetFloat("spotLight[5].cutOff", glm::cos(glm::radians(inner_angle[5])));
		shader->SetFloat("spotLight[5].outerCutOff", glm::cos(glm::radians(outer_angle[5])));
		shader->SetFloat("spotLight[5].falloff", falloff[5]);


		//Sphere7
		shader->SetVec3("lightColor[6]", lightColor[6]);
		shader->SetVec3("dirLight[6].position", position[6]);
		shader->SetVec3("dirLight[6].direction", -position[6]);
		shader->SetVec3("dirLight[6].ambient", ambient[6]);
		shader->SetVec3("dirLight[6].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("dirLight[6].specular", glm::vec3(0.5f, 0.5f, 0.5f));

		shader->SetVec3("pointLights[6].position", position[6]);
		shader->SetVec3("pointLights[6].ambient", ambient[6]);
		shader->SetVec3("pointLights[6].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("pointLights[6].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("pointLights[6].constant", 1.0f);
		shader->SetFloat("pointLights[6].linear", 0.09f);
		shader->SetFloat("pointLights[6].quadratic", 0.032f);

		shader->SetVec3("spotLight[6].position", position[6]);
		shader->SetVec3("spotLight[6].direction", position[6]);
		shader->SetVec3("spotLight[6].ambient", ambient[6]);
		shader->SetVec3("spotLight[6].diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader->SetVec3("spotLight[6].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("spotLight[6].constant", 1.0f);
		shader->SetFloat("spotLight[6].linear", 0.09f);
		shader->SetFloat("spotLight[6].quadratic", 0.032f);
		shader->SetFloat("spotLight[6].cutOff", glm::cos(glm::radians(inner_angle[6])));
		shader->SetFloat("spotLight[6].outerCutOff", glm::cos(glm::radians(outer_angle[6])));
		shader->SetFloat("spotLight[6].falloff", falloff[6]);


		//Sphere8
		shader->SetVec3("lightColor[7]", lightColor[7]);
		shader->SetVec3("dirLight[7].position", position[7]);
		shader->SetVec3("dirLight[7].direction", -position[7]);
		shader->SetVec3("dirLight[7].ambient", ambient[7]);
		shader->SetVec3("dirLight[7].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("dirLight[7].specular", glm::vec3(0.5f, 0.5f, 0.5f));

		shader->SetVec3("pointLights[7].position", position[7]);
		shader->SetVec3("pointLights[7].ambient", ambient[7]);
		shader->SetVec3("pointLights[7].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("pointLights[7].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("pointLights[7].constant", 1.0f);
		shader->SetFloat("pointLights[7].linear", 0.09f);
		shader->SetFloat("pointLights[7].quadratic", 0.032f);

		shader->SetVec3("spotLight[7].position", position[7]);
		shader->SetVec3("spotLight[7].direction", position[7]);
		shader->SetVec3("spotLight[7].ambient", ambient[7]);
		shader->SetVec3("spotLight[7].diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader->SetVec3("spotLight[7].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("spotLight[7].constant", 1.0f);
		shader->SetFloat("spotLight[7].linear", 0.09f);
		shader->SetFloat("spotLight[7].quadratic", 0.032f);
		shader->SetFloat("spotLight[7].cutOff", glm::cos(glm::radians(inner_angle[7])));
		shader->SetFloat("spotLight[7].outerCutOff", glm::cos(glm::radians(outer_angle[7])));
		shader->SetFloat("spotLight[7].falloff", falloff[7]);

		//Sphere9
		shader->SetVec3("lightColor[8]", lightColor[8]);
		shader->SetVec3("dirLight[8].position", position[8]);
		shader->SetVec3("dirLight[8].direction", -position[8]);
		shader->SetVec3("dirLight[8].ambient",  ambient[8]);
		shader->SetVec3("dirLight[8].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("dirLight[8].specular", glm::vec3(0.5f, 0.5f, 0.5f));

		shader->SetVec3("pointLights[8].position", position[8]);
		shader->SetVec3("pointLights[8].ambient", ambient[8]);
		shader->SetVec3("pointLights[8].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("pointLights[8].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("pointLights[8].constant", 1.0f);
		shader->SetFloat("pointLights[8].linear", 0.09f);
		shader->SetFloat("pointLights[8].quadratic", 0.032f);

		shader->SetVec3("spotLight[8].position", position[8]);
		shader->SetVec3("spotLight[8].direction", position[8]);
		shader->SetVec3("spotLight[8].ambient", ambient[8]);
		shader->SetVec3("spotLight[8].diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader->SetVec3("spotLight[8].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("spotLight[8].constant", 1.0f);
		shader->SetFloat("spotLight[8].linear", 0.09f);
		shader->SetFloat("spotLight[8].quadratic", 0.032f);
		shader->SetFloat("spotLight[8].cutOff", glm::cos(glm::radians(inner_angle[8])));
		shader->SetFloat("spotLight[8].outerCutOff", glm::cos(glm::radians(outer_angle[8])));
		shader->SetFloat("spotLight[8].falloff", falloff[8]);


		//Sphere10
		shader->SetVec3("lightColor[9]", lightColor[9]);
		shader->SetVec3("dirLight[9].position", position[9]);
		shader->SetVec3("dirLight[9].direction", -position[9]);
		shader->SetVec3("dirLight[9].ambient", ambient[9]);
		shader->SetVec3("dirLight[9].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("dirLight[9].specular", glm::vec3(0.5f, 0.5f, 0.5f));

		shader->SetVec3("pointLights[9].position", position[9]);
		shader->SetVec3("pointLights[9].ambient", ambient[9]);
		shader->SetVec3("pointLights[9].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("pointLights[9].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("pointLights[9].constant", 1.0f);
		shader->SetFloat("pointLights[9].linear", 0.09f);
		shader->SetFloat("pointLights[9].quadratic", 0.032f);

		shader->SetVec3("spotLight[9].position", position[9]);
		shader->SetVec3("spotLight[9].direction", position[9]);
		shader->SetVec3("spotLight[9].ambient", ambient[9]);
		shader->SetVec3("spotLight[9].diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader->SetVec3("spotLight[9].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("spotLight[9].constant", 1.0f);
		shader->SetFloat("spotLight[9].linear", 0.09f);
		shader->SetFloat("spotLight[9].quadratic", 0.032f);
		shader->SetFloat("spotLight[9].cutOff", glm::cos(glm::radians(inner_angle[9])));
		shader->SetFloat("spotLight[9].outerCutOff", glm::cos(glm::radians(outer_angle[9])));
		shader->SetFloat("spotLight[9].falloff", falloff[9]);


		//Sphere11
		shader->SetVec3("lightColor[10]", lightColor[10]);
		shader->SetVec3("dirLight[10].position", position[10]);
		shader->SetVec3("dirLight[10].direction", -position[10]);
		shader->SetVec3("dirLight[10].ambient", ambient[10]);
		shader->SetVec3("dirLight[10].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("dirLight[10].specular", glm::vec3(0.5f, 0.5f, 0.5f));

		shader->SetVec3("pointLights[10].position", position[10]);
		shader->SetVec3("pointLights[10].ambient", ambient[10]);
		shader->SetVec3("pointLights[10].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("pointLights[10].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("pointLights[10].constant", 1.0f);
		shader->SetFloat("pointLights[10].linear", 0.09f);
		shader->SetFloat("pointLights[10].quadratic", 0.032f);

		shader->SetVec3("spotLight[10].position", position[10]);
		shader->SetVec3("spotLight[10].direction", position[10]);
		shader->SetVec3("spotLight[10].ambient", ambient[10]);
		shader->SetVec3("spotLight[10].diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader->SetVec3("spotLight[10].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("spotLight[10].constant", 1.0f);
		shader->SetFloat("spotLight[10].linear", 0.09f);
		shader->SetFloat("spotLight[10].quadratic", 0.032f);
		shader->SetFloat("spotLight[10].cutOff", glm::cos(glm::radians(inner_angle[10])));
		shader->SetFloat("spotLight[10].outerCutOff", glm::cos(glm::radians(outer_angle[10])));
		shader->SetFloat("spotLight[10].falloff", falloff[10]);


		//Sphere12
		shader->SetVec3("lightColor[11]", lightColor[11]);
		shader->SetVec3("dirLight[11].position", position[11]);
		shader->SetVec3("dirLight[11].direction", -position[11]);
		shader->SetVec3("dirLight[11].ambient", ambient[11]);
		shader->SetVec3("dirLight[11].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("dirLight[11].specular", glm::vec3(0.5f, 0.5f, 0.5f));

		shader->SetVec3("pointLights[11].position", position[11]);
		shader->SetVec3("pointLights[11].ambient", ambient[11]);
		shader->SetVec3("pointLights[11].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("pointLights[11].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("pointLights[11].constant", 1.0f);
		shader->SetFloat("pointLights[11].linear", 0.09f);
		shader->SetFloat("pointLights[11].quadratic", 0.032f);

		shader->SetVec3("spotLight[11].position", position[11]);
		shader->SetVec3("spotLight[11].direction", position[11]);
		shader->SetVec3("spotLight[11].ambient", ambient[11]);
		shader->SetVec3("spotLight[11].diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader->SetVec3("spotLight[11].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("spotLight[11].constant", 1.0f);
		shader->SetFloat("spotLight[11].linear", 0.09f);
		shader->SetFloat("spotLight[11].quadratic", 0.032f);
		shader->SetFloat("spotLight[11].cutOff", glm::cos(glm::radians(inner_angle[11])));
		shader->SetFloat("spotLight[11].outerCutOff", glm::cos(glm::radians(outer_angle[11])));
		shader->SetFloat("spotLight[11].falloff", falloff[11]);


		//Sphere13
		shader->SetVec3("lightColor[12]", lightColor[12]);
		shader->SetVec3("dirLight[12].position", position[12]);
		shader->SetVec3("dirLight[12].direction", -position[12]);
		shader->SetVec3("dirLight[12].ambient", ambient[12]);
		shader->SetVec3("dirLight[12].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("dirLight[12].specular", glm::vec3(0.5f, 0.5f, 0.5f));

		shader->SetVec3("pointLights[12].position", position[12]);
		shader->SetVec3("pointLights[12].ambient", ambient[12]);
		shader->SetVec3("pointLights[12].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("pointLights[12].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("pointLights[12].constant", 1.0f);
		shader->SetFloat("pointLights[12].linear", 0.09f);
		shader->SetFloat("pointLights[12].quadratic", 0.032f);

		shader->SetVec3("spotLight[12].position", position[12]);
		shader->SetVec3("spotLight[12].direction", position[12]);
		shader->SetVec3("spotLight[12].ambient", ambient[12]);
		shader->SetVec3("spotLight[12].diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader->SetVec3("spotLight[12].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("spotLight[12].constant", 1.0f);
		shader->SetFloat("spotLight[12].linear", 0.09f);
		shader->SetFloat("spotLight[12].quadratic", 0.032f);
		shader->SetFloat("spotLight[12].cutOff", glm::cos(glm::radians(inner_angle[12])));
		shader->SetFloat("spotLight[12].outerCutOff", glm::cos(glm::radians(outer_angle[12])));
		shader->SetFloat("spotLight[12].falloff", falloff[12]);


		//Sphere14
		shader->SetVec3("lightColor[13]", lightColor[13]);
		shader->SetVec3("dirLight[13].position", position[13]);
		shader->SetVec3("dirLight[13].direction", -position[13]);
		shader->SetVec3("dirLight[13].ambient", ambient[13]);
		shader->SetVec3("dirLight[13].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("dirLight[13].specular", glm::vec3(0.5f, 0.5f, 0.5f));

		shader->SetVec3("pointLights[13].position", position[13]);
		shader->SetVec3("pointLights[13].ambient", ambient[13]);
		shader->SetVec3("pointLights[13].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("pointLights[13].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("pointLights[13].constant", 1.0f);
		shader->SetFloat("pointLights[13].linear", 0.09f);
		shader->SetFloat("pointLights[13].quadratic", 0.032f);

		shader->SetVec3("spotLight[13].position", position[13]);
		shader->SetVec3("spotLight[13].direction", position[13]);
		shader->SetVec3("spotLight[13].ambient", ambient[13]);
		shader->SetVec3("spotLight[13].diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader->SetVec3("spotLight[13].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("spotLight[13].constant", 1.0f);
		shader->SetFloat("spotLight[13].linear", 0.09f);
		shader->SetFloat("spotLight[13].quadratic", 0.032f);
		shader->SetFloat("spotLight[13].cutOff", glm::cos(glm::radians(inner_angle[13])));
		shader->SetFloat("spotLight[13].outerCutOff", glm::cos(glm::radians(outer_angle[13])));
		shader->SetFloat("spotLight[13].falloff", falloff[13]);


		//Sphere15
		shader->SetVec3("lightColor[14]", lightColor[14]);
		shader->SetVec3("dirLight[14].position", position[14]);
		shader->SetVec3("dirLight[14].direction", -position[14]);
		shader->SetVec3("dirLight[14].ambient", ambient[14]);
		shader->SetVec3("dirLight[14].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("dirLight[14].specular", glm::vec3(0.5f, 0.5f, 0.5f));

		shader->SetVec3("pointLights[14].position", position[14]);
		shader->SetVec3("pointLights[14].ambient", ambient[14]);
		shader->SetVec3("pointLights[14].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("pointLights[14].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("pointLights[14].constant", 1.0f);
		shader->SetFloat("pointLights[14].linear", 0.09f);
		shader->SetFloat("pointLights[14].quadratic", 0.032f);

		shader->SetVec3("spotLight[14].position", position[14]);
		shader->SetVec3("spotLight[14].direction", position[14]);
		shader->SetVec3("spotLight[14].ambient", ambient[14]);
		shader->SetVec3("spotLight[14].diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader->SetVec3("spotLight[14].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("spotLight[14].constant", 1.0f);
		shader->SetFloat("spotLight[14].linear", 0.09f);
		shader->SetFloat("spotLight[14].quadratic", 0.032f);
		shader->SetFloat("spotLight[14].cutOff", glm::cos(glm::radians(inner_angle[14])));
		shader->SetFloat("spotLight[14].outerCutOff", glm::cos(glm::radians(outer_angle[14])));
		shader->SetFloat("spotLight[14].falloff", falloff[14]);


		//Sphere16
		shader->SetVec3("lightColor[15]", lightColor[15]);
		shader->SetVec3("dirLight[15].position", position[15]);
		shader->SetVec3("dirLight[15].direction", -position[15]);
		shader->SetVec3("dirLight[15].ambient", ambient[15]);
		shader->SetVec3("dirLight[15].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("dirLight[15].specular", glm::vec3(0.5f, 0.5f, 0.5f));

		shader->SetVec3("pointLights[15].position", position[15]);
		shader->SetVec3("pointLights[15].ambient", ambient[15]);
		shader->SetVec3("pointLights[15].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		shader->SetVec3("pointLights[15].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("pointLights[15].constant", 1.0f);
		shader->SetFloat("pointLights[15].linear", 0.09f);
		shader->SetFloat("pointLights[15].quadratic", 0.032f);

		shader->SetVec3("spotLight[15].position", position[15]);
		shader->SetVec3("spotLight[15].direction", position[15]);
		shader->SetVec3("spotLight[15].ambient", ambient[15]);
		shader->SetVec3("spotLight[15].diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader->SetVec3("spotLight[15].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetFloat("spotLight[15].constant", 1.0f);
		shader->SetFloat("spotLight[15].linear", 0.09f);
		shader->SetFloat("spotLight[15].quadratic", 0.032f);
		shader->SetFloat("spotLight[15].cutOff", glm::cos(glm::radians(inner_angle[15])));
		shader->SetFloat("spotLight[15].outerCutOff", glm::cos(glm::radians(outer_angle[15])));
		shader->SetFloat("spotLight[15].falloff", falloff[15]);

		shader->SetFloat("material.shininess", 22.0f);
	}
};