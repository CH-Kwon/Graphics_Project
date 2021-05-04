#version 330 core

out vec4 FragColor;

in vec3 color;

void main()
{
	vec3 result = color;
	FragColor = vec4(color, 1.f);
}