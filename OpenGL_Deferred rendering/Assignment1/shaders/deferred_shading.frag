#version 330 core

out vec4 FragColor;

struct Material
{
    float shininess;
}; 

struct PointLight 
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	float Radius;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
	float falloff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;  

	float Radius;
};

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define PI 3.141592654
#define DOUBLEPI  6.283185308

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform int LightType;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gAlbedoDiff;

uniform int DepthCopy;

uniform vec3 color;
uniform vec3 lightColor[16];

uniform sampler2D Texture_1;
uniform sampler2D Texture_2;
uniform int projtype;
uniform int NormalCalculation;

uniform int sphere_max;
uniform vec3 viewPos;

uniform PointLight pointLights[16];
uniform DirLight dirLight[16];
uniform SpotLight spotLight[16];

uniform Material material;

void main()
{    
	vec3 FragPos = texture(gPosition, TexCoord).rgb;
	vec3 Normal = texture(gNormal, TexCoord).rgb;
	vec3 Diffuse = texture(gAlbedoDiff, TexCoord).rgb;
	float Specular = texture(gAlbedoSpec, TexCoord).r;

	vec3 lighting = Diffuse * 0.1;

	if(LightType == 0) //PointLight
	{

		for(int i = 0; i < sphere_max; i++)
		{
			float distance = length(pointLights[i].position - FragPos);
	
			if(distance < pointLights[i].Radius)
			{
				//diffuse
				vec3 lightDir = normalize(FragPos - pointLights[i].position);
				vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lightColor[i];
	
				//specular
				vec3 reflectDir = 2 * dot(lightDir, Normal) * Normal - lightDir;
				float spec = pow(max(dot(Normal, reflectDir),0.0), 16.0);
				vec3 specular = lightColor[i] * spec * Specular;
	
				//attenuation
				float attenuation = 1.0f/(1.0f + pointLights[i].linear * distance + pointLights[i].quadratic * distance * distance);
	
				diffuse *= attenuation;
				specular *= attenuation;
				lighting += diffuse + specular;	
			}
		}
	}
	else if(LightType == 1) //DirectionalLight
	{
		for(int i = 0; i < sphere_max; i++)
		{	
			//diffuse
			vec3 lightDir = normalize(-dirLight[i].direction);
			vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lightColor[i];
	
			//specular
			vec3 reflectDir = 2 * dot(lightDir, Normal) * Normal - lightDir;
			float spec = pow(max(dot(Normal, reflectDir),0.0), 16.0);
			vec3 specular = lightColor[i] * spec * Specular;
	
			//ambient
			vec3 ambient = dirLight[i].ambient * lightColor[i];

			lighting += diffuse + specular + ambient;			
		}
	}
	else if(LightType == 2) //SpotLight
	{
		for(int i = 0; i < sphere_max; i++)
		{	
			float distance = length(spotLight[i].position - FragPos);
	
			if(distance < spotLight[i].Radius)
			{
				//diffuse
				vec3 lightDir = normalize(FragPos - spotLight[i].position);
				float diff = max(dot(Normal, lightDir), 0.0);
	
				//specular
				vec3 reflectDir = 2 * dot(lightDir, Normal) * Normal - lightDir;
				float spec = pow(max(dot(Normal, reflectDir),0.0), 16.0);

				float distance = length(spotLight[i].position - FragPos);
				float attenuation = 1.0 / (spotLight[i].constant + spotLight[i].linear * distance + spotLight[i].quadratic * (distance * distance));    

				float theta = dot(lightDir, normalize(-spotLight[i].direction)); 
			    float epsilon = spotLight[i].cutOff - spotLight[i].outerCutOff;
				float intensity = pow(clamp((spotLight[i].cutOff - theta) / epsilon, 0.0, 1.0), spotLight[i].falloff);
	
				vec3 ambient = spotLight[i].ambient * material.shininess * lightColor[i];
				vec3 diffuse = spotLight[i].diffuse * diff *attenuation * intensity * lightColor[i];
				vec3 specular = spotLight[i].specular * spec *attenuation * intensity * lightColor[i];
			
				lighting += diffuse + specular + ambient;
			}
		}
	}

    if(DepthCopy == 0)
	{
		FragColor = vec4(FragPos, 1.0);
	}
	else if(DepthCopy == 1)
	{
		FragColor = vec4(Normal, 1.0);
	}
	else if(DepthCopy == 2)
	{
		FragColor = texture(gAlbedoDiff, TexCoord);
	}
	else if(DepthCopy == 3)
	{
		FragColor = texture(gAlbedoSpec, TexCoord);
	}
	else if(DepthCopy == 4)
	{
		FragColor = vec4(lighting, 1.0);
	}
}
