#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;
out vec3 FragPos;

uniform mat4 projection;
uniform mat4 viewtwo;
uniform mat4 model;

void main()
{
    TexCoords = aPos;
    FragPos = vec3(model * vec4(aPos,1.0));
	vec4 position = projection * viewtwo * vec4(aPos, 1.0);
    gl_Position = position.xyww;
}  