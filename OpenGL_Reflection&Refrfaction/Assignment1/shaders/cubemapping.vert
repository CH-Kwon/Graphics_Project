#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = aNormal; 
    vec4 position = projection * view * model * vec4(aPos, 1.0);
    gl_Position = position;
}