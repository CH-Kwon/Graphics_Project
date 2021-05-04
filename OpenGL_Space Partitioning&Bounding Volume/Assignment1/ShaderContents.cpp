/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ShaderContents.cpp
Purpose: For using shaders
Language: c, c++
Platform: visual studio 2017, window
Project: chiho.kwon_CS300_1
Author: <name : Chiho Kwon, student login : chiho.kwon ,student id : 180003018
Creation date: 09/28/2018
End Header --------------------------------------------------------*/

#include <string>
#include "Shader.h"

std::string Shader::m_vertexShader = {

	R"glsl(

    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;

    out vec3 FragPos;
    out vec3 Normal;

    uniform mat4 model;
    uniform mat4 projection;
    uniform mat4 view;
	
    void main()
    {
        FragPos = vec3(model*vec4(aPos, 1.0));
        Normal = mat3(transpose(inverse(model))) *aNormal;
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
	
)glsl"
};

std::string Shader::m_fragmentShader = {

R"glsl(

    #version 330 core
    out vec4 FragColor;

    in vec3 FragPos;
    in vec3 Normal;
    uniform vec3 color;

    uniform vec3 lightColor;
    uniform vec3 lightPosition;

    uniform vec3 viewPos;

    uniform float ambientStrength;
    uniform float specularStrength;

    void main()
    {  
		vec3 viewDir = normalize(viewPos - FragPos);

        vec3 lightDir = normalize(lightPosition - FragPos);

	    vec3 norm = normalize(Normal);

		float diff = max(dot(norm, lightDir), 0.0);

		vec3 reflectDir = reflect(-lightDir, norm);

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);

		vec3 mixUp = lightColor;

		vec3 ambient = ambientStrength * mixUp;
		vec3 diffuse = diff * mixUp;
		vec3 specular = specularStrength * spec * mixUp;

		vec3 result = (ambient + diffuse + specular) * color;

		FragColor = vec4(result, 1.f);
    }
	
)glsl"

};

std::string Shader::m_vertexShader_2 = {

   R"glsl(

    #version 330 core
    layout (location = 0) in vec3 aPos;
   layout (location = 1) in vec3 aNormal;

   out VS_OUT {
      vec3 normal;
   } vs_out;
   out vec3 outColor;

   uniform mat4 model;   
   uniform mat4 view;
   uniform mat4 projection;
   
    void main()
    {
        mat3 normalMatrix = mat3(transpose(inverse(view * model)));
      vs_out.normal = vec3(projection * vec4(normalMatrix * aNormal, 0.0));
      gl_Position = projection * view * model * vec4(aPos, 1.0);
      outColor = vec3(1.0, 0.0, 0.0);
   }
)glsl"
};
std::string Shader::m_geometryShader = {

   R"glsl(

    #version 330 core
   layout (triangles) in;
   layout (line_strip, max_vertices = 6) out;
   
   in VS_OUT {
      vec3 normal;
   } gs_in[];
   out vec3 outColor;
   const float MAGNITUDE = 0.2;

   void GenerateLine(int index)
   {
       gl_Position = gl_in[index].gl_Position;
      outColor = vec3(1.0, 1.0, 0.0);
       EmitVertex();
       gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
      outColor = vec3(1.0, 1.0, 0.0);
       EmitVertex();
       EndPrimitive();
   }
   void main()
   {
      GenerateLine(0); // first vertex normal
       GenerateLine(1); // second vertex normal
       GenerateLine(2); // third vertex normal
   }    
)glsl"
};
std::string Shader::m_geometryShader_2 = {

   R"glsl(

    #version 330 core
   layout (triangles) in;
   layout (line_strip, max_vertices = 2) out;
   
   in VS_OUT {
      vec3 normal;
   } gs_in[];
   
   const float MAGNITUDE = 0.2;

   out vec3 outColor;

   void main()
   {
      vec3 V0 = gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz;
      vec3 V1 = gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz;
      
      vec3 N = normalize(cross(V0, V1));

      vec4 P = (gl_in[0].gl_Position + gl_in[1].gl_Position
                        + gl_in[2].gl_Position) / 3.0;
      
      gl_Position = P;
      outColor = vec3(0.0, 1.0, 1.0);
      EmitVertex();
      
      gl_Position = P + vec4( N , 0.0 ) * MAGNITUDE;
      outColor = vec3(0.0, 1.0, 1.0);
      EmitVertex();
      EndPrimitive();
   }    
)glsl"
};
std::string Shader::m_fragmentShader_2 = {
   R"glsl(
   #version 330 core
   out vec4 FragColor;
   in vec3 outColor;
   
   void main()
   {
       FragColor = vec4(outColor, 1.0);
   }
)glsl"
};