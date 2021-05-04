/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Purpose: for window and opengl funtion process
Language: c, c++
Platform: visual studio 2017, window
Project: chiho.kwon_CS350_2
Author: <name : Chiho Kwon, student login : chiho.kwon ,student id : 180003018
Creation date: 2/24/2019
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
#include "bvhTree.h"
#include "Octree.h"
#include "bspTree.h"

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
int bvh_drawType = 0;
bool isbvh = false;

bool isOctree = false;
bool isBsptree = false;

enum BV_Methods { DEFAULT,AABB, CENTROID, RITTER, LARSSON, BS_PCA, ELLIPSE_PCA, OBB_PCA, ROOT, DEPTH1, DEPTH2, DEPTH3};
BV_Methods BV_methods;

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

Camera cam(glm::vec3(0.0f, 1.5f, 11.0f));

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

	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();

	glEnable(GL_DEPTH_TEST);

	struct ModelData
	{
		const char* model_name[6] = { "models\\teapot_mid_poly.obj", "models\\sphere_mid_poly.obj", "models\\teapot_mid_poly.obj",
									  "models\\sphere_mid_poly.obj", "models\\teapot_mid_poly.obj", "models\\sphere_mid_poly.obj"};
		

		glm::vec3 modelpos[6] = { glm::vec3(-3.5f, -2.f, -3.0f), glm::vec3(4.5f, 0.f, -3.5f), glm::vec3(-3.f, 0.f, 3.0f),
								 glm::vec3(3.f, 0.f, 4.f), glm::vec3(-1.5f, 3.f, 0.f), glm::vec3(1.5f,3.f,1.f)};

		glm::vec3 color[6];
		glm::vec3 scale[6];
	};
	ModelData model_data;

	struct BV
	{
		const char* bv_sphere = "models\\sphere_mid_poly.obj";
		const char* bv_box = "models\\cube_low_poly.obj";
		float ritter_rad;
	};
	BV bv_data;
	
	for (int i = 0; i < 6; i++)
	{
		model_data.color[i] = glm::vec3(0.1f, 0.1f, 0.1f);
		model_data.scale[i] = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	Shader shader_vert_norm(true, Shader::VERTEX);
	Shader shader_face_norm(true, Shader::FACE);

	Shader shader(false, Shader::NORMAL);
	Shader shader_deffered(false, Shader::DEFFERED);
	Shader shader_gbuffer(false, Shader::G_BUFFER);
	Shader shader_octree(false, Shader::OCTREE);

	Model positon_model;
	//Draw object
	Model Object[6];
	std::vector<Model*> model_vec;
	std::vector<glm::vec3> vertex_chunk;
	std::vector<Triangle> world_triangle;
	std::vector<int> obj_index;
	std::vector<std::vector<unsigned>> obj_indices;

	Tree bvh_tree;
	int tree_depth = 1;

	for (int i = 0; i < 6; i++)
	{
		Object[i].position = model_data.modelpos[i];
		Object[i].color = model_data.color[i];
		Object[i].LoadObj(model_data.model_name[i], true, false, model_data.modelpos[i]);
		if (!Object[i].loadPPM("models\\metal_roof_diff_512x512.ppm", Object[i].TextDiffInfo))
		{
			std::cout << "PPM diffuse file cannot open" << std::endl;
		}

		if (!Object[i].loadPPM("models\\metal_roof_spec_512x512.ppm", Object[i].TextSpecInfo))
		{
			std::cout << "PPM spec file cannot open" << std::endl;
		}
		Object[i].Draw(Object[i].out_vertices, Object[i].vertex_indices, Object[i].textureUV);
		Object[i].scale = model_data.scale[i];
		Object[i].TextureProcess();

		for (unsigned int j = 0; j < Object[i].out_vertices.size(); j++)
		{
			vertex_chunk.push_back(Object[i].out_vertices[j] + Object[i].position);
		}

		model_vec.push_back(&Object[i]);

		for(unsigned int j = 0; j < Object[i].m_triangle.size(); ++j)
		{
			Triangle worldTri_temp = Object[i].m_triangle[j];
			worldTri_temp.pt[0] *= Object[i].scale;
			worldTri_temp.pt[0] += Object[i].position;

			worldTri_temp.pt[1] *= Object[i].scale;
			worldTri_temp.pt[1] += Object[i].position;

			worldTri_temp.pt[2] *= Object[i].scale;
			worldTri_temp.pt[2] += Object[i].position;

			world_triangle.push_back(worldTri_temp);
		}
		
		
		for (unsigned int j = 0; j < Object[i].vertex_indices.size(); j+=3)
		{
			std::vector<unsigned> tri_;
			tri_.push_back(Object[i].vertex_indices[j]);
			tri_.push_back(Object[i].vertex_indices[j+1]);
			tri_.push_back(Object[i].vertex_indices[j+2]);
			obj_index.push_back(i);
			obj_indices.push_back(tri_);
		}
		

		Object[i].octree_color.resize(Object[i].out_vertices.size());
		Object[i].bsptree_color.resize(Object[i].out_vertices.size());
	}

	Node *node;
	bvh_tree.TopDownBVTree(&node, vertex_chunk, tree_depth);

	Model TreeBox;
	TreeBox.LoadObj(bv_data.bv_box, false, true, node->bv_mid);
	TreeBox.Draw(TreeBox.out_vertices, TreeBox.vertex_indices, TreeBox.textureUV);

	Model TreeCircle;
	TreeCircle.LoadObj(bv_data.bv_sphere, false, true, node->bv_mid);
	TreeCircle.Draw(TreeCircle.out_vertices, TreeCircle.vertex_indices, TreeCircle.textureUV);

	OctreeNode* oc_node = BuildOctree(model_vec, GetCenter(model_vec), GetLongest(model_vec),8);
	//oc_node = new OctreeNode;
	BSPNode* bsp_node = BuildBSPTree(world_triangle, model_vec, 8, obj_index, obj_indices);

	Model Octree;
	Octree.LoadObj(bv_data.bv_box, false, true, oc_node->octree_center);
	Octree.Draw(Octree.out_vertices, Octree.vertex_indices, Octree.textureUV);

	Model FillBsptree[6];
	for (unsigned int i = 0; i < 6; i++)
	{
		FillBsptree[i].bsptree_color = Object[i].bsptree_color;
		FillBsptree[i].position = Object[i].position;
		FillBsptree[i].scale = Object[i].scale;
		FillBsptree[i].LoadObj(model_data.model_name[i], false, false, model_data.modelpos[i]);
		FillBsptree[i].DrawBsptree(FillBsptree[i].out_vertices, FillBsptree[i].vertex_indices, FillBsptree[i].textureUV);
	}

	Model FillOctree[6];
	for (unsigned int i = 0; i < 6; i++)
	{
		FillOctree[i].octree_color = Object[i].octree_color;
		FillOctree[i].position = Object[i].position;
		FillOctree[i].scale = Object[i].scale;
		FillOctree[i].LoadObj(model_data.model_name[i], false, false, model_data.modelpos[i]);
		FillOctree[i].DrawOctree(FillOctree[i].out_vertices, FillOctree[i].vertex_indices, FillOctree[i].textureUV);
	}

	Model AABB[6];
	for (int i = 0; i < 6; i++)
	{
		AABB[i].position = model_data.modelpos[i];
		AABB[i].color = glm::vec3(1.f, 0.f, 0.f);
		AABB[i].scale = Object[i].aabb();
		AABB[i].LoadObj(bv_data.bv_box, false, true, AABB[i].position);
		AABB[i].Draw(AABB[i].out_vertices, AABB[i].vertex_indices, AABB[i].textureUV);
	}

	Model Centroid[6];
	for (int i = 0; i < 6; i++)
	{
		glm::vec3 cent_scale = Object[i].GetCentroid();
		Centroid[i].position = model_data.modelpos[i];
		Centroid[i].color = glm::vec3(1.f, 0.f, 0.f);
		Centroid[i].scale = cent_scale;
		Centroid[i].LoadObj(bv_data.bv_sphere, false, true, Centroid[i].position);
		Centroid[i].Draw(Centroid[i].out_vertices, Centroid[i].vertex_indices, Centroid[i].textureUV);
	}

	Model Ritter[6];
	for (int i = 0; i < 6; i++) 
	{
		float rad = Object[i].GetRitter();
		Ritter[i].position = model_data.modelpos[i];
		Ritter[i].color = glm::vec3(1.f, 0.f, 0.f);
		Ritter[i].scale = glm::vec3(rad, rad, rad);
		Ritter[i].LoadObj(bv_data.bv_sphere, false, true, Ritter[i].position);
		Ritter[i].Draw(Ritter[i].out_vertices, Ritter[i].vertex_indices, Ritter[i].textureUV);
	}

	Model EOPS[6];
	for (int i = 0; i < 6; i++)
	{
		float rad = Object[i].GetEOPS();
		EOPS[i].position = model_data.modelpos[i];
		EOPS[i].color = glm::vec3(1.f, 0.f, 0.f);
		EOPS[i].scale = glm::vec3(rad, rad, rad);
		EOPS[i].LoadObj(bv_data.bv_sphere, false, true, EOPS[i].position);
		EOPS[i].Draw(EOPS[i].out_vertices, EOPS[i].vertex_indices, EOPS[i].textureUV);
	}

	Model PCA_SPHERE[6];
	for (int i = 0; i < 6; i++)
	{
		float rad = Object[i].EigenSphere();
		PCA_SPHERE[i].position = model_data.modelpos[i];
		PCA_SPHERE[i].color = glm::vec3(1.f, 0.f, 0.f);
		PCA_SPHERE[i].scale = glm::vec3(rad, rad, rad);
		PCA_SPHERE[i].LoadObj(bv_data.bv_sphere, false, true, PCA_SPHERE[i].position);
		PCA_SPHERE[i].Draw(PCA_SPHERE[i].out_vertices, PCA_SPHERE[i].vertex_indices, PCA_SPHERE[i].textureUV);
	}

	Model Ellipsoid[6];
	for (int i = 0; i < 6; i++)
	{
		Ellipsoid[i].position = model_data.modelpos[i];
		Ellipsoid[i].color = glm::vec3(1.f, 0.f, 0.f);
		Ellipsoid[i].scale = Object[i].Ellipsoid();
		Ellipsoid[i].m_v = Object[i].m_v;
		Ellipsoid[i].LoadObj(bv_data.bv_sphere, false, true, Ellipsoid[i].position);
		Ellipsoid[i].Draw(Ellipsoid[i].out_vertices, Ellipsoid[i].vertex_indices, Ellipsoid[i].textureUV);
	}

	Model OBB[6];
	for (int i = 0; i < 6; i++)
	{
		OBB[i].position = model_data.modelpos[i];
		OBB[i].color = glm::vec3(1.f, 0.f, 0.f);
		OBB[i].scale = Object[i].Ellipsoid();
		OBB[i].m_v = Object[i].m_v;
		OBB[i].LoadObj(bv_data.bv_box, false, true, OBB[i].position);
		OBB[i].Draw(OBB[i].out_vertices, OBB[i].vertex_indices, OBB[i].textureUV);
	}
	
	//light spheres
	Model Object2[16];
	Object2[0].LoadObj("models\\sphere_high_poly.obj", false, false, glm::vec3(0,0,0));
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
	Model quadRender;

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
			//Toggle Bounding volume algorithms
			if (ImGui::CollapsingHeader("Bounding Volume Algorithms"))
			{
				if (ImGui::Button("DEFAULT"))
				{
					BV_methods = BV_Methods::AABB;
				}
				if (ImGui::Button("AABB"))
				{
					BV_methods = BV_Methods::AABB;
				}
				if (ImGui::Button("CENTROID"))
				{
					BV_methods = BV_Methods::CENTROID;
				}
				if (ImGui::Button("RITTER"))
				{
					BV_methods = BV_Methods::RITTER;
				}
				if (ImGui::Button("LARSSON"))
				{
					BV_methods = BV_Methods::LARSSON;
				}
				if (ImGui::Button("BS_PCA"))
				{
					BV_methods = BV_Methods::BS_PCA;
				}
				if (ImGui::Button("ELLIPSE_PCA"))
				{
					BV_methods = BV_Methods::ELLIPSE_PCA;
				}
				if (ImGui::Button("OBB_PCA"))
				{
					BV_methods = BV_Methods::OBB_PCA;
				}
			}
			if (ImGui::CollapsingHeader("Spatial Partitioning"))
			{
				if (ImGui::Button("Draw Octree"))
				{
					isOctree = true;
				}
				if (ImGui::Button("ShutDown Octree"))
				{
					isOctree = false;
				}
				if (ImGui::Button("Draw Bsptree"))
				{
					isBsptree = true;
				}
				if (ImGui::Button("ShutDown Bsptree"))
				{
					isBsptree = false;
				}
			}
			ImGui::End();

			ImGui::Begin("Imgui2");
			ImGui::Text("Depth Control");

			if (ImGui::CollapsingHeader("BVH Depth Control"))
			{
				if (ImGui::Button("Show top down BVH"))
				{
					isbvh = true;
				}
				if (ImGui::Button("Shut down top down BVH"))
				{
					isbvh = false;
				}
				
				ImGui::SliderInt("Top Down Depth", &tree_depth, 1, 7, "%d");			
			}
			if (ImGui::CollapsingHeader("BVH Depth Drawing Type"))
			{
				if (ImGui::Button("BOX"))
				{
					bvh_drawType = 0;
				}
				if (ImGui::Button("SPHERE"))
				{
					bvh_drawType = 1;
				}
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
		for (int i = 0; i < 6; i++)
		{
			if (!isOctree || !isBsptree)
			{
				Object[i].TextureData(&shader_gbuffer);
				Object[i].Render(&cam, &shader_gbuffer, aspect, Object[i].position, false, false);
			}
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
		quadRender.renderQuad();

		//Bind Frame buffers
		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); 
		glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);	
		
		//Drawing light spheres

		shader.Use();
		
		if (isbvh == true)
		{
			if (bvh_drawType == 0)
			{
				node->DrawBox(tree_depth, &TreeBox, &cam, &shader, aspect, true, false, bvh_drawType);
			}
			else if (bvh_drawType == 1)
			{
				node->DrawBox(tree_depth, &TreeCircle, &cam, &shader, aspect, true, false, bvh_drawType);
			}
		}
		 
		if (isOctree == true)
		{
			oc_node->DrawBox(&Octree, &cam, &shader, aspect, true, false, 8);
			shader_octree.Use();
			for (int i = 0; i < 6; i++)
			{
				FillOctree[i].octree_color = Object[i].octree_color;
				FillOctree[i].RenderPartitionColor(&cam, &shader_octree, aspect, FillOctree[i].position);
			}
		}

		if (isBsptree == true)
		{	
			shader_octree.Use();
			for (int i = 0; i < 6; i++)
			{
				FillBsptree[i].bsptree_color = Object[i].bsptree_color;
				FillBsptree[i].RenderBspColor(&cam, &shader_octree, aspect, FillBsptree[i].position);
			}
		}

		shader.Use();
		for (int i = 0; i < 6; i++)
		{	
			if (BV_methods == BV_Methods::AABB)
			{
				AABB[i].Render(&cam, &shader, aspect, AABB[i].position, true, false);
			}
			else if (BV_methods == BV_Methods::CENTROID)
			{
				Centroid[i].Render(&cam, &shader, aspect, Centroid[i].position, true, false);
			}
			else if (BV_methods == BV_Methods::RITTER)
			{
				Ritter[i].Render(&cam, &shader, aspect, Ritter[i].position, true, false);
			}
			else if (BV_methods == BV_Methods::LARSSON)
			{
				EOPS[i].Render(&cam, &shader, aspect, EOPS[i].position, true, false);
			}
			else if (BV_methods == BV_Methods::BS_PCA)
			{
				PCA_SPHERE[i].Render(&cam, &shader, aspect, PCA_SPHERE[i].position, true, false);
			}
			else if (BV_methods == BV_Methods::ELLIPSE_PCA)
			{
				Ellipsoid[i].Render(&cam, &shader, aspect, Ellipsoid[i].position, true, true);
			}
			else if (BV_methods == BV_Methods::OBB_PCA)
			{
				OBB[i].Render(&cam, &shader, aspect, OBB[i].position, true, true);
			}			
		}
			
		for (sphere_data.sphere_count = 0; sphere_data.sphere_count < directionalLight.sphere_max; sphere_data.sphere_count++)
		{
			directionalLight.position[sphere_data.sphere_count] = Object2[sphere_data.sphere_count].position;
			Object2[sphere_data.sphere_count].color = directionalLight.lightColor[sphere_data.sphere_count];
			Object2[sphere_data.sphere_count].Render(&cam, &shader, aspect, Object2[sphere_data.sphere_count].position,false,false);
		}
	
		//Rotating lightSpheres, controlling numbers
		if (sphere_data.sphere_rotating)
		{
			count += (float)0.005f; //speed of light spheres
			for (sphere_data.sphere_count = 0; sphere_data.sphere_count < directionalLight.sphere_max; ++sphere_data.sphere_count)
			{
				Object2[sphere_data.sphere_count].position.x = 7.0f*(float)sin(count + (PI*sphere_data.sphere_count / 8));
				Object2[sphere_data.sphere_count].position.z = 7.0f*(float)cos(count + (PI*sphere_data.sphere_count / 8));
			}
		}
		
		//For Vertnormal Toggle
		if (normal_data.showVertexNorm == true)
		{
			shader_vert_norm.Use();
			for (int i = 0; i < 6; i++)
			{
				Object[i].Render(&cam, &shader_vert_norm, aspect, Object[i].position, false,false);
			}
		}
		//For Facenormal Toggle
		if (normal_data.showFaceNorm == true)
		{
			shader_face_norm.Use();
			for (int i = 0; i < 6; i++)
			{
				Object[i].Render(&cam, &shader_face_norm, aspect, Object[i].position, false,false);
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