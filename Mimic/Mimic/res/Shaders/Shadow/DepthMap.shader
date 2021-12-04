#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// This shader is used to fill depth buffer 
// mvp is from the perspective of light source
// This shader should have no output

void main()
{
	gl_Position = projection * view * model * vec4(aPos , 1.0f);
}




#shader fragment
#version 330 core



void main()
{
}