#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec2 texCoord0;
out vec3 normal0;

uniform mat4 MVP;



void main()
{
// apply MVP
	gl_Position = MVP * vec4(position, 1.0);
	texCoord0 = texCoord;
	normal0 = normal;
}
