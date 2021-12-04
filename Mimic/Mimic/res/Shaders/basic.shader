#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord = aTexCoords;
}







#shader fragment
#version 330 core

in vec2 TexCoord;
uniform sampler2D tex;

out vec4 FragColor;


void main()
{
	//FragColor = vec4(0.8, 0.8, 0.8, 1.0f);
	FragColor = texture(tex, TexCoord);
}