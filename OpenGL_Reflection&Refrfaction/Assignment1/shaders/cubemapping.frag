#version 400 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
///////////////////////////////////light///////

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform int sphere_max;

uniform vec3 color;
uniform vec3 lightColor[16];
uniform PointLight pointLights[16];
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 light_Color);

///////////////////////////////////////////////////////
uniform vec3 viewPos;
uniform sampler2D skybox[6];
uniform int isReflect;
uniform int isRefract;
uniform int isPhong;
uniform float Material_val;

vec4 Reflection(vec3 m_vec);
vec4 Refraction(vec3 m_vec);

vec3 CalReflection();
vec3 CalRefraction();

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 light_Color)
{
    vec3 lightDir = normalize(fragPos - light.position);

    float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = 2*dot(normal,lightDir)*normal - lightDir;

    float spec = pow(max(dot(normal, reflectDir), 0.0), 22.0);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

	vec3 ambient = light.ambient * light_Color;
	vec3 diffuse = light.diffuse * diff * light_Color;
	vec3 specular = light.specular * spec * light_Color;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
}

vec3 CalReflection()
{
	vec3 Incidence_vec = normalize(FragPos - viewPos);
	vec3 reflection_vec = 2*dot(Incidence_vec, Normal)*Normal-Incidence_vec;

	return reflection_vec;
}

vec3 CalRefraction()
{
	float ratio = 1.0 / Material_val;
	vec3 I_vec = normalize(viewPos - FragPos);
	float k_val = 1.0 - ratio * ratio * (1.0 - dot(Normal,I_vec)*dot(Normal,I_vec));
	vec3 refraction_vec = (ratio * I_vec) - (ratio * dot(Normal, I_vec) + sqrt(k_val))*Normal;

	return refraction_vec;
}

vec4 Reflection(vec3 m_vec)
{
	vec4 Final;
	vec2 resultCoord;

	//m_vec = CalReflection();

	float absX = abs(m_vec.x);
	float absY = abs(m_vec.y);
	float absZ = abs(m_vec.z);

	int isXPositive = m_vec.x > 0 ? 1 : 0;
	int isYPositive = m_vec.y > 0 ? 1 : 0;
	int isZPositive = m_vec.z > 0 ? 1 : 0;

	//right
	if(isXPositive == 0 && absX >= absY && absX >= absZ)
	{
		resultCoord.x = 0.5*(-m_vec.z / absX + 1.0);
		resultCoord.y = 0.5*(-m_vec.y / absX + 1.0);
		Final = texture(skybox[4], resultCoord);
	}
	//left
	else if(isXPositive == 1 && absX >= absY && absX >= absZ)
	{
		resultCoord.x = 0.5*(m_vec.z / absX + 1.0);
		resultCoord.y = 0.5*(-m_vec.y / absX + 1.0);
		Final = texture(skybox[3], resultCoord);
	}
	//top
	else if(isYPositive == 0 && absY >= absX && absY >= absZ)
	{
		resultCoord.x = 0.5*(m_vec.z / absY + 1.0);
		resultCoord.y = 0.5*(-m_vec.x / absY + 1.0);
		Final = texture(skybox[1], resultCoord);
	}
	//bottom
	else if(isYPositive == 1 && absY >= absX && absY >= absZ)
	{
		resultCoord.x = 0.5*(-m_vec.z / absY + 1.0);
		resultCoord.y = 0.5*(-m_vec.x / absY + 1.0);
		Final = texture(skybox[2], resultCoord);
	}
	//back
	else if(isZPositive == 0 && absZ >= absX && absZ >= absY)
	{
		resultCoord.x = 0.5*(m_vec.x / absZ + 1.0);
		resultCoord.y = 0.5*(-m_vec.y / absZ + 1.0);
		Final = texture(skybox[0], resultCoord);
	}
	//front
	else if(isZPositive == 1 && absZ >= absX && absZ >= absY)
	{
		resultCoord.x = 0.5*(-m_vec.x / absZ + 1.0);
		resultCoord.y = 0.5*(-m_vec.y / absZ + 1.0);
		Final = texture(skybox[5], resultCoord);
	}
	return Final;
}

vec4 Refraction(vec3 m_vec)
{
	vec4 Final;
	vec2 resultCoord;
	
	//m_vec = CalRefraction();
	
	float absX = abs(m_vec.x);
	float absY = abs(m_vec.y);
	float absZ = abs(m_vec.z);

	int isXPositive = m_vec.x > 0 ? 1 : 0;
	int isYPositive = m_vec.y > 0 ? 1 : 0;
	int isZPositive = m_vec.z > 0 ? 1 : 0;

	//left
	if(isXPositive == 0 && absX >= absY && absX >= absZ)
	{
		resultCoord.x = 0.5*(-m_vec.z / absX + 1.0);
		resultCoord.y = 0.5*(-m_vec.y / absX + 1.0);
		Final = texture(skybox[4], resultCoord);
	}
	//right
	else if(isXPositive == 1 && absX >= absY && absX >= absZ)
	{
		resultCoord.x = 0.5*(m_vec.z / absX + 1.0);
		resultCoord.y = 0.5*(-m_vec.y / absX + 1.0);
		Final = texture(skybox[3], resultCoord);
	}
	//bottom
	else if(isYPositive == 0 && absY >= absX && absY >= absZ)
	{
		resultCoord.x = 0.5*(m_vec.z / absY + 1.0);
		resultCoord.y = 0.5*(-m_vec.x / absY + 1.0);
		Final = texture(skybox[1], resultCoord);
	}
	//top
	else if(isYPositive == 1 && absY >= absX && absY >= absZ)
	{
		resultCoord.x = 0.5*(-m_vec.z / absY + 1.0);
		resultCoord.y = 0.5*(-m_vec.x/ absY + 1.0);
		Final = texture(skybox[2], resultCoord);
	}
	//back
	else if(isZPositive == 0 && absZ >= absX && absZ >= absY)
	{
		resultCoord.x = 0.5*(m_vec.x / absZ + 1.0);
		resultCoord.y = 0.5*(-m_vec.y / absZ + 1.0);
		Final = texture(skybox[0], resultCoord);
	}
	//front
	else if(isZPositive == 1 && absZ >= absX && absZ >= absY)
	{
		resultCoord.x = 0.5*(-m_vec.x / absZ + 1.0);
		resultCoord.y = 0.5*(-m_vec.y / absZ + 1.0);
		Final = texture(skybox[5], resultCoord);
	}
	return Final;
}

void main()
{    
	float Ratio = 1.0 / Material_val;
	const float FresnelPower = 5.0;
	float F = ((1.0-Ratio)*(1.0-Ratio))/((1.0 + Ratio)*(1.0 + Ratio));
	vec3 I_vec = normalize(viewPos - FragPos);

	Ratio = F + (1.0 - F) * pow((1.0 - dot(-I_vec, Normal)),FresnelPower);

	vec4 result;
	vec3 m_vec;

	vec3 reflec_vec = CalReflection();
	vec3 refrac_vec = CalRefraction();

	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 color_result;

	for(int i=0; i < sphere_max; i++)
	{
		color_result += CalcPointLight(pointLights[i], Normal, FragPos, viewDir, lightColor[i]);
	}
	vec3 new_vec;

    if(isReflect == 1 && isRefract == 0)
	{
		if(isPhong == 0)
		{
			m_vec = CalReflection();
			result = Reflection(m_vec);
		}
		else
		{
			m_vec = CalReflection();
			result = Reflection(m_vec);
			new_vec = vec3(result);
			result = vec4(mix(new_vec, color_result, 0.5),1);
		}
	}
	else if(isRefract == 1 && isReflect == 0)
	{
		if(isPhong == 0)
		{
			m_vec = CalRefraction();
			result = Refraction(m_vec);
		}
		else
		{
			m_vec = CalRefraction();
			result = Refraction(m_vec);
			new_vec = vec3(result);
			result = vec4(mix(new_vec, color_result, 0.5),1);
		}
	}
	else if(isReflect == 1 && isRefract == 1)
	{
		if(isPhong == 0)
		{
			vec3 m_vec1 = CalReflection();
			vec3 m_vec2 = CalRefraction();

			vec4 result1 = Reflection(m_vec1);
			vec4 result2 = Refraction(m_vec2);

			result = mix(result2, result1, Ratio);	
		}
		else
		{
			vec3 m_vec1 = CalReflection();
			vec3 m_vec2 = CalRefraction();

			vec4 result1 = Reflection(m_vec1);
			vec4 result2 = Refraction(m_vec2);

			result = mix(result2, result1, Ratio);	
			new_vec = vec3(result);
			result = vec4(mix(new_vec, color_result, 0.5),1);
		}
	}

	FragColor = result;
}