/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.cpp
Purpose: For using camera
Language: c, c++
Platform: visual studio 2017, window
Project: chiho.kwon_CS300_3
Author: <name : Chiho Kwon, student login : chiho.kwon ,student id : 180003018
Creation date: 11/08/2018
End Header --------------------------------------------------------*/

#include "Camera.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 _position, glm::vec3 _up,
	float _yaw, float _pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	position = _position;
	worldUp = _up;
	yaw = _yaw;
	pitch = _pitch;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::Update(Shader* _shader)
{
	_shader->SetVec3("viewPos", position);
	updateCameraVectors();
}

void Camera::ProcessKeyboard(Cam_Moving _direction, float _deltaTime)
{
	float velocity = movementSpeed * _deltaTime;

	if (_direction == FORWARD) {
		position += front * velocity;
	}

	if (_direction == BACKWARD) {
		position -= front * velocity;
	}

	if (_direction == LEFT) {
		position -= right * velocity;
	}

	if (_direction == RIGHT) {
		position += right * velocity;
	}

	if (_direction == R) {
		yaw *= velocity;
	}

	if (_direction == L)
	{
		yaw *= -velocity;
	}

	if (_direction == UP)
	{
		pitch *= velocity;
	}

	if (_direction == DOWN)
	{
		pitch *= -velocity;
	}
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainpitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (constrainpitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	if (zoom >= 1.0f && zoom <= 45.0f)
		zoom -= yoffset;
	if (zoom <= 1.0f)
		zoom = 1.0f;
	if (zoom >= 45.0f)
		zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	glm::vec3 _front;
	_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	_front.y = sin(glm::radians(pitch));
	_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(_front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}