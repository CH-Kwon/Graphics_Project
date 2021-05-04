/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Purpose: for window and opengl funtion process
Language: c, c++
Platform: visual studio 2017, window
Project: chiho.kwon_CS300_3
Author: <name : Chiho Kwon, student login : chiho.kwon ,student id : 180003018
Creation date: 11/08/2018
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

bool isObjLoad = false;

bool scenario1 = false;
bool scenario2 = false;
bool scenario3 = false;

bool showVertexNorm = false;
bool showFaceNorm = false;

bool sphere_rotating = true;
int sphere_min = 0;
int sphere_count = 0;

float count = 0.f;

bool isReflect = true;
bool isRefract = false;
bool isPhong = false;

float material_val;

float Air = 1.000293f;
float Hydrogen = 1.000132f;
float Water = 1.333f;
float OliveOil = 1.47f;
float Ice = 1.31f;
float Quartz = 1.46f;
float Diamond = 2.42f;
float Acrylic_plexiglas_Lucite = 1.49f;

Camera cam(glm::vec3(0.0f, 1.2f, 7.0f));

struct GLFWwindow;

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow *window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();

	Shader normal_shader(false, Shader::NORMAL);

	Shader phong_shading_shader(false, Shader::PHONG_SHADING);

	Shader skybox_shader(false, Shader::SKYBOX);
	Shader cube_shader(false, Shader::CUBEMAP);
	
	const char* model_name = "models\\sphere_high_poly.obj";
	isReflect = true;
	isRefract = false;
	material_val = Air;

	Model Skybox;
	Skybox.LoadObj("models\\cube_low_poly.obj");
	Skybox.position = { 0, 0, 0 };
	Skybox.color = { 0.0f, 0.7f, 0.0f };
	Skybox.scale = { 1, 1, 1 };
	Skybox.Draw(Skybox.out_vertices, Skybox.vertex_indices);

	std::vector<std::string> faces
	{
		{"TGA\\badomen_lf.tga",
		 "TGA\\badomen_rt.tga",
		 "TGA\\badomen_dn.tga",
		 "TGA\\badomen_up.tga",
		 "TGA\\badomen_bk.tga",
		 "TGA\\badomen_ft.tga"
		 }
	};

	Skybox.loadTexture(faces);

	Model Object;
	Object.LoadObj(model_name);
	Object.position = { 0, 0, 0 };
	Object.color = { 0.0f, 0.f, 0.f };
	Object.scale = { 1.f, 1.f, 1.f };
	Object.Draw(Object.out_vertices, Object.vertex_indices);

	Model Object2[16];
	Object2[0].LoadObj("models\\sphere_high_poly.obj");
	Object2[0].position = { 1, 0, 0 };
	Object2[0].color = { 0.0f, 0.0f, 0.0f };
	Object2[0].scale = { 0.1f, 0.1f, 0.1f };
	Object2[0].Draw(Object2[0].out_vertices, Object2[0].vertex_indices);

	for (int i = 1; i < 16; i++)
	{
		Object2[i].out_vertices = Object2[0].out_vertices;
		Object2[i].vertex_indices = Object2[0].vertex_indices;
		Object2[i].normalVector = Object2[0].normalVector;
		Object2[i].scale = Object2[0].scale;
	}

	for (int i = 1; i < 16; i++)
	{
		Object2[i].Draw(Object2[i].out_vertices, Object2[i].vertex_indices);
	}

	for(unsigned i = 0; i < 2; ++i)
	{
		Object2[i].position.x = 2.0f*sin(PI*i);
		Object2[i].position.z = 2.0f*cos(PI*i);
	}

	Light directionalLight;

	bool show_demo_window = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	unsigned int textureColorbuffer[6];
	
	for (int index = 0; index < sizeof(textureColorbuffer) / sizeof(unsigned int); index++)
	{
		glGenTextures(1, &textureColorbuffer[index]);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer[index]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureColorbuffer[index], 0);
	}
	
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Width, Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		dt = currentFrame - last_Frame;
		last_Frame = currentFrame;

		ProcessInput(window);

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
		{
			static float f = 0.0f;
			static int counter = 0;
			int sphere_index = 0;

			ImGui::Begin("Imgui");
			ImGui::Text("Menu");

			if (ImGui::CollapsingHeader("Obj Loader")) ////////////////////objs
			{
				if (ImGui::Button("bunny_high_poly.obj"))
				{
					model_name = "models\\bunny_high_poly.obj";
					isObjLoad = true;
				}
				if (ImGui::Button("cube_high_poly.obj"))
				{
					model_name = "models\\cube_high_poly.obj";
					isObjLoad = true;
				}
				if (ImGui::Button("horse_high_poly.obj"))
				{
					model_name = "models\\horse_high_poly.obj";
					isObjLoad = true;
				}
				if (ImGui::Button("menger_sponge_level_1_high_poly.obj"))
				{
					model_name = "models\\menger_sponge_level_1_high_poly.obj";
					isObjLoad = true;
				}
				if (ImGui::Button("sphere_high_poly.obj"))
				{
					model_name = "models\\sphere_high_poly.obj";
					isObjLoad = true;
				}
				if (ImGui::Button("sphere_mid_poly.obj"))
				{
					model_name = "models\\sphere_mid_poly.obj";
					isObjLoad = true;
				}
				if (ImGui::Button("teapot_mid_poly.obj"))
				{
					model_name = "models\\teapot_mid_poly.obj";
					isObjLoad = true;
				}
			}
			if (ImGui::CollapsingHeader("Control Rotating"))
			{
				ImGui::Checkbox("Rotating", &sphere_rotating);
			}
			
			ImGui::End();

			ImGui::Begin("Imgui2");
			ImGui::Text("ObjectStatus");
			if (ImGui::Button("Reflection"))
			{
				isReflect = 1;
				isRefract = 0;
			}
			if (ImGui::Button("Refraction"))
			{	
				isReflect = 0;
				isRefract = 1;
			}
			if (ImGui::Button("Mixing"))
			{
				isReflect = 1;
				isRefract = 1;
			}
			if (ImGui::Button("Use Phong + Environment"))
			{
				isPhong = 1;
			}
			if (ImGui::Button("Not Use Phong + Environment"))
			{
				isPhong = 0;
			}

			ImGui::End();

			ImGui::Begin("Imgui3");
			ImGui::Text("Materials");

			if (ImGui::Button("Air"))
			{
				material_val = Air;
			}
			if (ImGui::Button("Hydrogen"))
			{
				material_val = Hydrogen;
			}
			if (ImGui::Button("Water"))
			{
				material_val = Water;
			}
			if (ImGui::Button("OliveOil"))
			{
				material_val = OliveOil;
			}
			if (ImGui::Button("Ice"))
			{
				material_val = Ice;
			}
			if (ImGui::Button("Quartz"))
			{
				material_val = Quartz;
			}
			if (ImGui::Button("Diamond"))
			{
				material_val = Diamond;
			}
			if (ImGui::Button("Acrylic_plexiglas_Lucite"))
			{
				material_val = Acrylic_plexiglas_Lucite;
			}
			ImGui::SliderFloat("Refractive Index", &material_val, 0.f, 100.f);

			ImGui::End();
		}

		if (isObjLoad == true)
		{
			Object.out_vertices.clear();
			Object.vertex_indices.clear();

			Object.LoadObj(model_name);
			Object.Draw(Object.out_vertices, Object.vertex_indices);
			isObjLoad = false;
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glm::vec3 snapshot_pos = cam.position;
		float pitch_info = cam.pitch;
		float yaw_info = cam.yaw;

		for (int index = 0; index < sizeof(textureColorbuffer) / sizeof(unsigned int); index++)
		{
			cam.position = glm::vec3(0, 0, 0);

			if (index == 0)
			{
				//left
				cam.yaw = 270.f;
				cam.pitch = 0.f;
				cam.Update(&skybox_shader);
			}
			else if (index == 1)
			{
				//right
				cam.yaw = 90.f;
				cam.pitch = 0.f;
				cam.Update(&skybox_shader);
			}
			else if (index == 2)
			{
				//bottom
				cam.yaw = 0.f;
				cam.pitch = 90.f;
				cam.Update(&skybox_shader);
			}
			else if (index == 3)
			{
				//top
				cam.yaw = 0.f;
				cam.pitch = 270.f;
				cam.Update(&skybox_shader);
			}
			else if (index == 4)
			{
				//back
				cam.yaw = 180.f;
				cam.pitch = 0.f;
				cam.Update(&skybox_shader);
			}
			else if (index == 5)
			{
				//front
				cam.pitch = 0.f;
				cam.yaw = 0.f;
				cam.Update(&skybox_shader);
			}
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);

			skybox_shader.Use();
			cam.zoom = 90.f;
			Skybox.TextureData(&skybox_shader, Skybox.textureID[0]);
			Skybox.Render(&cam, &skybox_shader, 1.f, Skybox.position);

			for (sphere_count = 0; sphere_count < directionalLight.sphere_max; sphere_count++)
			{
				normal_shader.Use();
				Object2[sphere_count].color = directionalLight.lightColor[sphere_count];
				Object2[sphere_count].Render(&cam, &normal_shader, 1.f, Object2[sphere_count].position);
			}
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureColorbuffer[index], 0);
		}
		cam.zoom = 45.f;
		cam.position = snapshot_pos;
		cam.pitch = pitch_info;
		cam.yaw = yaw_info;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		skybox_shader.Use();
		cam.Update(&skybox_shader);
		Skybox.TextureData(&skybox_shader, Skybox.textureID[0]);
		Skybox.Render(&cam, &skybox_shader, aspect, Skybox.position);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		cube_shader.Use();
		cube_shader.SetInt("isReflect", isReflect);
		cube_shader.SetInt("isRefract", isRefract);
		cube_shader.SetFloat("Material_val", material_val);
		cube_shader.SetInt("isPhong", isPhong);
		cam.Update(&cube_shader);
		Object.TextureData(&cube_shader, textureColorbuffer[0]);
		Object.Render(&cam, &cube_shader, aspect, Object.position);

		directionalLight.Update(&cube_shader);
		//around spheres
		for (sphere_count = 0; sphere_count < directionalLight.sphere_max; sphere_count++)
		{
			normal_shader.Use();
			Object2[sphere_count].color = directionalLight.lightColor[sphere_count];
			Object2[sphere_count].Render(&cam, &normal_shader, aspect, Object2[sphere_count].position);
		}
		//around spheres
		if (sphere_rotating)
		{
			count += (float)0.004f;
			for (sphere_count = 0; sphere_count < directionalLight.sphere_max; ++sphere_count)
			{
				Object2[sphere_count].position.x = 2.0f*(float)sin(count + (PI*sphere_count ));
				Object2[sphere_count].position.z = 2.0f*(float)cos(count + (PI*sphere_count ));
				directionalLight.position[sphere_count] = Object2[sphere_count].position;
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

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(width);
	UNREFERENCED_PARAMETER(height);
	glViewport(0, 0, width, height);
}

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

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(xoffset);
	UNREFERENCED_PARAMETER(yoffset);

	cam.ProcessMouseScroll(float(yoffset));
}

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
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cam.ProcessKeyboard(R, dt);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cam.ProcessKeyboard(L, dt);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cam.ProcessKeyboard(UP, dt);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cam.ProcessKeyboard(DOWN, dt);
}