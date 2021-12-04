#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord = aTexCoords;
	Normal = aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0));
}







#shader fragment
#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D tex;
uniform vec3 viewPos;

out vec4 FragColor;


void main()
{
	// hard coded light config
	vec3 lightPos = vec3(3, -13, -4);
	vec3 lightColor = vec3(2, 2, 2);
	vec3 ambient = vec3(0.1, 0.1, 0.1);
	float specularStrength = 0.5;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	// diffuse
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	// attenuation
	float distance = length(lightPos - FragPos);
	float attenuation = 1.0 / (1.0 + 0.09f * distance + 0.032f * (distance * distance));


	vec3 objColor = texture(tex, TexCoord).rgb;

	vec3 result = attenuation * (ambient + diffuse + specular) * objColor;

	//FragColor = vec4(0.8, 0.8, 0.8, 1.0f);
	FragColor = vec4(result, 1.0);
}