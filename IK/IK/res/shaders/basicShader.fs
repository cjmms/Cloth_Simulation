#version 330 core

in vec2 texCoord0;
in vec3 normal0;

out vec4 FragColor;

// setup light pos, color
vec3 lightDir = vec3(2, 2, 0);
vec3 lightColor = vec3(1, 1, 1);
vec3 objectColor = vec3(1, 0.2, 0);

void main()
{
	float diff = max(dot(normal0, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	float ambient = 0.1;

	// ambient + diffuse, part of phong
	vec3 result = (ambient + diffuse) * objectColor;

	// add color at the edge of obj
	if (texCoord0.x>0.95||texCoord0.y>0.95||texCoord0.y<=0.05||texCoord0.x<=0.05)
		result=vec3(0,0,1);

	FragColor = vec4(result,1);
}
