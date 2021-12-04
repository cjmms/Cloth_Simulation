#shader vertex
#version 330 core
layout(Location = 0) in vec2 aPos;
layout(Location = 1) in vec2 aTextureCoord;

out vec2 TextureCoord;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
	TextureCoord = aTextureCoord;
}


//  This shader is designed to show color buffer

#shader fragment
#version 330 core

in vec2 TextureCoord;
out vec4 FragColor;

uniform sampler2D map;


void main()
{
	FragColor = texture(map, TextureCoord);

	// only first channel
	float c = texture(map, TextureCoord).r;
	FragColor = vec4(c, c, c, 1.0);


	// visualize depth
	//FragColor = vec4(vec3(texture(map, TextureCoord).r ), 1.0);

	
	//FragColor = vec4(1.0f, 0, 0, 0);
}