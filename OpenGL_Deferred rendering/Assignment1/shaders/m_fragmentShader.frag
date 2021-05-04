#version 330 core
    out vec4 FragColor;

    in vec3 FragPos;
    in vec3 Normal;
	
	struct PointLight 
	{
		vec3 position;
		vec3 diffuse;
	};
	
    uniform vec3 color;
    uniform vec3 lightColor;
	uniform PointLight pointLights[16];
	
    void main()
    {  
        vec3 lightDir = normalize(pointLights[0].position - FragPos);
	    vec3 norm = normalize(Normal);
		vec3 result = color;
		float diff = max(dot(norm, lightDir), 0.0);

		vec3 l_color = lightColor;
		
		vec3 diffuse = pointLights[0].diffuse * diff * l_color;
		
		result += diffuse;

		FragColor = vec4(result, 1.f);
    }