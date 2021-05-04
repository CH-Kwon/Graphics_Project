#version 330 core

out vec4 FragColor;

struct Material {
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
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
};

in vec3 FragPos;
in vec3 Normal;
uniform vec3 color;
uniform vec3 lightColor[16];

uniform bool isDir,isPoint,isSpot;
uniform int sphere_max;
uniform vec3 viewPos;

uniform DirLight dirLight[16];
uniform PointLight pointLights[16];
uniform SpotLight spotLight[16];

uniform Material material;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 light_Color);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 light_Color);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 light_Color);

void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result = color;
	for(int i=0; i < sphere_max; i++)
	{
		if(isDir == true)
		{
			result += CalcDirLight(dirLight[i], norm, viewDir, lightColor[i]);
		}
		if(isPoint == true)
		{
			result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, lightColor[i]);    
		}
		if(isSpot == true)
		{
			result += CalcSpotLight(spotLight[i], norm, FragPos, viewDir, lightColor[i]); 
		}
    } 
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 light_Color)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = (2*(dot(normal,lightDir)*normal)) - lightDir;
    float spec = pow(max(dot(normal, reflectDir), 0.0), material.shininess);

	 vec3 ambient = light.ambient * light_Color;
	 vec3 diffuse = light.diffuse * (diff * light_Color);
	 vec3 specular = light.specular * (spec * light_Color);
	
	 return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 light_Color)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = 2*dot(normal,lightDir)*normal - lightDir;

    float spec = pow(max(dot(normal, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

	vec3 ambient = light.ambient * attenuation * light_Color;
	vec3 diffuse = light.diffuse * diff * attenuation * light_Color;
	vec3 specular = light.specular * spec * attenuation * light_Color;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 light_Color)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = 2*dot(normal,lightDir)*normal - lightDir;
    float spec = pow(max(dot(normal, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = pow(clamp((light.cutOff - theta) / epsilon, 0.0, 1.0), light.falloff);
	
	vec3 ambient = light.ambient *attenuation * intensity * light_Color;
	vec3 diffuse = light.diffuse * diff *attenuation * intensity * light_Color;
	vec3 specular = light.specular * spec *attenuation * intensity * light_Color;

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	
	return (ambient + diffuse + specular);
}