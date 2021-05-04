#version 330 core
out vec4 FragColor;

in vec3 TexCoords;
in vec3 FragPos;

uniform sampler2D skybox[6];

vec4 convert_xyz_to_cube(vec3 m_vec);

vec4 convert_xyz_to_cube(vec3 m_vec)
{
	vec4 Final = vec4(0,0,0,0);
	vec2 resultCoord = vec2(0,0);

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
		Final = texture(skybox[0], resultCoord);
	}
	//right
	else if(isXPositive == 1 && absX >= absY && absX >= absZ)
	{
		resultCoord.x = 0.5*(m_vec.z / absX + 1.0);
		resultCoord.y = 0.5*(-m_vec.y / absX + 1.0);
		Final = texture(skybox[1], resultCoord);
	}
	//bottom
	else if(isYPositive == 0 && absY >= absX && absY >= absZ)
	{
		resultCoord.x = 0.5*(m_vec.z / absY + 1.0);
		resultCoord.y = 0.5*(-m_vec.x / absY + 1.0);
		Final = texture(skybox[2], resultCoord);
	}
	//top
	else if(isYPositive == 1 && absY >= absX && absY >= absZ)
	{
		resultCoord.x = 0.5*(m_vec.z / absY + 1.0);
		resultCoord.y = 0.5*(m_vec.x / absY + 1.0);
		Final = texture(skybox[3], resultCoord);
	}
	//back
	else if(isZPositive == 0 && absZ >= absX && absZ >= absY)
	{
		resultCoord.x = 0.5*(m_vec.x / absZ + 1.0);
		resultCoord.y = 0.5*(-m_vec.y / absZ + 1.0);
		Final = texture(skybox[4], resultCoord);
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
    FragColor = convert_xyz_to_cube(FragPos);
}
