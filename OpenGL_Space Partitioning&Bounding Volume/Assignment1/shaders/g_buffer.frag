#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gAlbedoDiff;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D Texture_1;
uniform sampler2D Texture_2;

float FindMax(vec3 m_vec)
{
	float max_ = abs(m_vec.x);
	if(abs(m_vec.y) > max_)
		max_ = m_vec.y;
	if(abs(m_vec.z) > max_)
		max_ = m_vec.z;
	return max_;
}
vec2 Projection(vec3 normals)
{
	vec2 resultCoord = vec2(0,0);
	float MaxVal = FindMax(normals);
    if(MaxVal == normals.x)
    {
    	resultCoord.x = -normals.z / MaxVal;
    	resultCoord.y = normals.y / MaxVal;
    }
    else if(MaxVal == -normals.x)
    {
    	resultCoord.x = normals.z / MaxVal;
    	resultCoord.y = normals.y / MaxVal;
    }
    else if(MaxVal == normals.y)
    {
    	resultCoord.x = normals.x / MaxVal;
    	resultCoord.y = -normals.z / MaxVal;
    }
    else if(MaxVal == -normals.y)
    {
    	resultCoord.x = normals.x / MaxVal;
    	resultCoord.y = normals.z / MaxVal;
    }
    else if(MaxVal == normals.z)
    {
    	resultCoord.x = normals.x / MaxVal;
    	resultCoord.y = normals.y / MaxVal;
    }
    else if(MaxVal == -normals.z)
    {
    	resultCoord.x = -normals.x / MaxVal;
    	resultCoord.y = normals.y / MaxVal;
    }
	return resultCoord;
}

void main()
{    
	vec2 TexCoords_ = Projection(Normal);
    gPosition = FragPos;
 
    gNormal = normalize(Normal);
  
    gAlbedoSpec.rgb = texture(Texture_2, TexCoords_).rgb;

    gAlbedoDiff.rgb = texture(Texture_1, TexCoords_).rgb;
}