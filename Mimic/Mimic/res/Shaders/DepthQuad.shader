#shader vertex
#version 330 core
layout(Location = 0) in vec2 aPos;
layout(Location = 1) in vec2 aTextureCoord;

out vec2 TextureCoord;

void main() 
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
	TextureCoord = aTextureCoord;
}


//  This shader is designed to show depth buffer


#shader fragment
#version 330 core

in vec2 TextureCoord;
out vec4 FragColor;

uniform sampler2D map;


void main()
{
	float sampleZ = texture(map, TextureCoord).r;


	float near = 0.1;
	float far = 30;
	float z = sampleZ * 2.0 - 1.0; // back to NDC 
	float depth = (2.0 * near * far) / (far + near - z * (far - near)) / far;

	//FragColor = vec4(vec3(depth), 1.0f);
	FragColor = vec4(vec3(texture(map, TextureCoord).r / 30), 1.0f);
}