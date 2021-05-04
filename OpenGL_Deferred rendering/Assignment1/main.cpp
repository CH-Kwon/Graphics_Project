/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Purpose: for window and opengl funtion process
Language: c, c++
Platform: visual studio 2017, window
Project: chiho.kwon_CS350_1
Author: <name : Chiho Kwon, student login : chiho.kwon ,student id : 180003018
Creation date: 2/3/2019
End Header --------------------------------------------------------*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SPHERE 16
#define MIN_SPHERE 1

using namespace glm;

const char* glsl_version = "#version 330";

const float PI = 4.0f*atan(1.0f);
const unsigned  Width = 1024;
const unsigned  Height = 768;

float lastX = Width / 2.0f;
float lastY = Height / 2.0f;
float aspect = float(Width) / float(Height);
float dt, last_Frame = 0.f;
bool firstMouse = true;

struct Normal
{
	bool showVertexNorm = false;
	bool showFaceNorm = false;
};
Normal normal_data;

struct Spheres
{
	bool sphere_rotating = true;
	int sphere_min = 0;
	int sphere_count = 0;
};
Spheres sphere_data;

struct DefferedData
{
	bool showDeffered = true;
	int DepthCopy = 4;
};
DefferedData deffered_data;

float count = 0.f;

Camera cam(glm::vec3(0.0f, 1.5f, 7.0f));

struct GLFWwindow;

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow *window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void renderQuad();

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create Window
	GLFWwindow* window = glfwCreateWindow(Width, Height, "CS300", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();

	glEnable(GL_DEPTH_TEST);

	struct ModelData
	{
		const char* model_name[9] = { "models\\bunny_high_poly.obj", "models\\sphere_high_poly.obj", "models\\bunny_high_poly.obj",
									  "models\\sphere_high_poly.obj", "models\\teapot_mid_poly.obj", "models\\sphere_high_poly.obj",
									  "models\\teapot_mid_poly.obj", "models\\sphere_high_poly.obj", "models\\horse_high_poly.obj" };

		glm::vec3 modelpos[9] = { glm::vec3(-1.5f, 0.f, -1.5f), glm::vec3(0.f, 0.f, -1.5f), glm::vec3(1.5f, 0.f, -1.5f),
								 glm::vec3(-1.5f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.5f,0.f,0.f),
								 glm::vec3(-1.5f, 0.f, 1.5f), glm::vec3(0.f, 0.f, 1.5f), glm::vec3(1.5f,0.f,1.5f) };

		glm::vec3 color[9];
		glm::vec3 scale[9];
	};
	ModelData model_data;

	for (int i = 0; i < 9; i++)
	{
		model_data.color[i] = glm::vec3(0.1f, 0.1f, 0.1f);
		model_data.scale[i] = glm::vec3(0.5f, 0.5f, 0.5f);
	}

	Shader shader_vert_norm(true, Shader::VERTEX);
	Shader shader_face_norm(true, Shader::FACE);

	Shader shader(false, Shader::NORMAL);
	Shader shader_deffered(false, Shader::DEFFERED);
	Shader shader_gbuffer(false, Shader::G_BUFFER);

	//Draw object
	Model Object[9];

	for (int i = 0; i < 9; i++)
	{
		Object[i].position = model_data.modelpos[i];
		Object[i].color = model_data.color[i];
		Object[i].scale = model_data.scale[i];
		Object[i].LoadObj(model_data.model_name[i], true, model_data.modelpos[i]);
		if (!Object[i].loadPPM("models\\metal_roof_diff_512x512.ppm", Object[i].TextDiffInfo))
		{
			std::cout << "PPM diffuse file cannot open" << std::endl;
		}

		if (!Object[i].loadPPM("models\\metal_roof_spec_512x512.ppm", Object[i].TextSpecInfo))
		{
			std::cout << "PPM spec file cannot open" << std::endl;
		}
		Object[i].Draw(Object[i].out_vertices, Object[i].vertex_indices, Object[i].textureUV);
		Object[i].TextureProcess();
	}

	//light spheres
	Model Object2[16];
	Object2[0].LoadObj("models\\sphere_high_poly.obj", false, glm::vec3(0,0,0));
	Object2[0].position = { 1, 0, 0 };
	Object2[0].color = { 0.1f, 0.2f, 0.3f };
	Object2[0].scale = { 0.1f, 0.1f, 0.1f };
	Object2[0].Draw(Object2[0].out_vertices, Object2[0].vertex_indices, Object2[0].textureUV);

	for (int i = 1; i < 16; i++)
	{
		Object2[i].out_vertices = Object2[0].out_vertices;
		Object2[i].vertex_indices = Object2[0].vertex_indices;
		Object2[i].normalVector = Object2[0].normalVector;
		Object2[i].textureUV = Object2[0].textureUV;
		Object2[i].scale = Object2[0].scale;
	}

	for (int i = 1; i < 16; i++)
	{
		Object2[i].Draw(Object2[i].out_vertices, Object2[i].vertex_indices, Object2[i].textureUV );
	}

	for(unsigned i = 0; i < 16; ++i)
	{
		Object2[i].position.x = 4.0f*sin(PI*i / 8);
		Object2[i].position.z = 4.0f*cos(PI*i / 8);
	}

	Light directionalLight;

	bool show_demo_window = true;

	// g_buffer frame buffer
	unsigned int gBuffer;
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	unsigned int gPosition, gNormal, gAlbedoSpec, gAlbedoDiff;

	// Position Color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Width, Height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	//Normal Color Buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Width, Height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	//Color + Specular buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

	//Diffuse
	glGenTextures(1, &gAlbedoDiff);
	glBindTexture(GL_TEXTURE_2D, gAlbedoDiff);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gAlbedoDiff, 0);
	
	//Telling to OpenGL to which color attachment we will use
	unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);
	// Attachment of depth buffer
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Width, Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Deffered shader use
	shader_deffered.Use();
	shader_deffered.SetInt("gPosition", 0);
	shader_deffered.SetInt("gNormal", 1);
	shader_deffered.SetInt("gAlbedoSpec", 2);
	shader_deffered.SetInt("gAlbedoDiff", 3);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		dt = currentFrame - last_Frame;
		last_Frame = currentFrame;

		ProcessInput(window);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//ImGui stuffs
		if (show_demo_window)
		{
			static float f = 0.0f;
			static int counter = 0;
			int sphere_index = 0;


			ImGui::Begin("Imgui");
			ImGui::Text("Menu");

			if (ImGui::CollapsingHeader("Normals")) ///////////////////////normals
			{
				if (ImGui::Button("Show Vertex Normal"))
				{
					if (normal_data.showVertexNorm == true)
					{
						normal_data.showVertexNorm = false;
					}
					else
					{
						normal_data.showVertexNorm = true;
					}	
				}
				if (ImGui::Button("Show Face Normal"))
				{
					if (normal_data.showFaceNorm == true)
					{
						normal_data.showFaceNorm = false;
					}
					else
					{
						normal_data.showFaceNorm = true;
					}
				}
			}	
			if (ImGui::CollapsingHeader("Control Rotating"))
			{
				ImGui::Checkbox("Rotating", &sphere_data.sphere_rotating);
			}
			if (ImGui::CollapsingHeader("LightSphere Controller"))//////////////////load more orbit
			{
				ImGui::BulletText("Control numbers");
				if (ImGui::Button("  Add sphere  "))
				{
					++directionalLight.sphere_max;
					if (directionalLight.sphere_max >= MAX_SPHERE)
					{
						directionalLight.sphere_max = MAX_SPHERE;
					}
					directionalLight.lightColor[directionalLight.sphere_max - 1]
						= directionalLight.lightColor[directionalLight.sphere_max - 1];
				}
				ImGui::SameLine();
				if (ImGui::Button(" Remove sphere "))
				{
					--directionalLight.sphere_max;
					if (directionalLight.sphere_max <= MIN_SPHERE)
					{
						directionalLight.sphere_max = MIN_SPHERE;
					}
					directionalLight.lightColor[directionalLight.sphere_max] =
						directionalLight.lightColor[directionalLight.sphere_max];
				}

				for (int i = 0; i < directionalLight.sphere_max; i++)
				{
					std::string name = std::to_string(i);
					const char* pchar = name.c_str();
					if (ImGui::Button(pchar))
					{
						ImGui::SameLine();
					}
				}
			}
			ImGui::End();

			ImGui::Begin("Imgui2");
			ImGui::Text("Depth Control");

			if (ImGui::Button("Position"))
			{
				deffered_data.DepthCopy = 0;
			}
			if (ImGui::Button("Normal"))
			{
				deffered_data.DepthCopy = 1;
			}
			if (ImGui::Button("Diffuse"))
			{
				deffered_data.DepthCopy = 2;
			}
			if (ImGui::Button("Specular"))
			{
				deffered_data.DepthCopy = 3;
			}
			if (ImGui::Button("Combination"))
			{
				deffered_data.DepthCopy = 4;
			}

			ImGui::End();

			ImGui::Begin("Imgui3");
			ImGui::Text("LightTypes");

			if (ImGui::Button("PointLight"))
			{
				directionalLight.lightType = 0;
			}
			if (ImGui::Button("DirectionLight"))
			{
				directionalLight.lightType = 1;
			}
			if (ImGui::Button("SpotLight"))
			{
				directionalLight.lightType = 2;
			}

			ImGui::End();
		}

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//geometry pass
		shader_gbuffer.Use();
		for (int i = 0; i < 9; i++)
		{
			Object[i].TextureData(&shader_gbuffer);
			Object[i].Render(&cam, &shader_gbuffer, aspect, Object[i].position);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
		//lighting pass
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader_deffered.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gAlbedoDiff);


		shader_deffered.SetInt("DepthCopy", deffered_data.DepthCopy);

		//lighting update
		directionalLight.Update(&shader_deffered);
		//camera update
		cam.Update(&shader_deffered);

		//Rendering Quad
		renderQuad();

		//Bind Frame buffers
		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); 
		glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);	
		
		//Drawing light spheres
		shader.Use();
		for (sphere_data.sphere_count = 0; sphere_data.sphere_count < directionalLight.sphere_max; sphere_data.sphere_count++)
		{
			directionalLight.position[sphere_data.sphere_count] = Object2[sphere_data.sphere_count].position;
			Object2[sphere_data.sphere_count].color = directionalLight.lightColor[sphere_data.sphere_count];
			Object2[sphere_data.sphere_count].Render(&cam, &shader, aspect, Object2[sphere_data.sphere_count].position);
		}
	
		//Rotating lightSpheres, controlling numbers
		if (sphere_data.sphere_rotating)
		{
			count += (float)0.005f; //speed of light spheres
			for (sphere_data.sphere_count = 0; sphere_data.sphere_count < directionalLight.sphere_max; ++sphere_data.sphere_count)
			{
				Object2[sphere_data.sphere_count].position.x = 3.0f*(float)sin(count + (PI*sphere_data.sphere_count / 8));
				Object2[sphere_data.sphere_count].position.z = 3.0f*(float)cos(count + (PI*sphere_data.sphere_count / 8));
			}
		}

		//For Vertnormal Toggle
		if (normal_data.showVertexNorm == true)
		{
			shader_vert_norm.Use();
			for (int i = 0; i < 9; i++)
			{
				Object[i].Render(&cam, &shader_vert_norm, aspect, Object[i].position);
			}
		}
		//For Facenormal Toggle
		if (normal_data.showFaceNorm == true)
		{
			shader_face_norm.Use();
			for (int i = 0; i < 9; i++)
			{
				Object[i].Render(&cam, &shader_face_norm, aspect, Object[i].position);
			}
		}


		ImGui::Render();

		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

unsigned int quadVAO = 0;
unsigned int quadVBO;

//function for render quad
void renderQuad()
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

//FrameBuffer call back funtion
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(width);
	UNREFERENCED_PARAMETER(height);
	glViewport(0, 0, width, height);
}

//For mouse using
void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	UNREFERENCED_PARAMETER(window);

	float f_xpos = float(xpos);
	float f_ypos = float(ypos);

	if (firstMouse)
	{
		lastX = f_xpos;
		lastY = f_ypos;
		firstMouse = false;
	}

	float xoffset = f_xpos - lastX;
	float yoffset = lastY - f_ypos;

	lastX = f_xpos;
	lastY = f_ypos;

	cam.ProcessMouseMovement(xoffset, yoffset);
}

//Process mouse scroll call back function
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(xoffset);
	UNREFERENCED_PARAMETER(yoffset);

	cam.ProcessMouseScroll(float(yoffset));
}

//Input W,A,S,D to move camera
void ProcessInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.ProcessKeyboard(FORWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.ProcessKeyboard(BACKWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.ProcessKeyboard(LEFT, dt);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.ProcessKeyboard(RIGHT, dt);
}