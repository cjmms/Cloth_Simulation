#shader vertex
#version 330 core
layout(Location = 0) in vec2 aPos;
layout(Location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 0.0f, 1.0f);
    TexCoord = aTexCoord;
}





#shader fragment
#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D map;

uniform bool horizontal;

float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec2 tex_offset = 1.0 / textureSize(map, 0); // gets size of single texel
    vec4 result = texture(map, TexCoord) * weight[0]; // current fragment's contribution

    if (horizontal)
    {
        for (int i = 1; i < 5; ++i) // 4 texel to the left, 4 texels to the right
        {
            result += texture(map, TexCoord + vec2(tex_offset.x * i, 0.0)) * weight[i];
            result += texture(map, TexCoord - vec2(tex_offset.x * i, 0.0)) * weight[i];
        }
    }
    else
    {
        for (int i = 1; i < 5; ++i) // 4 texel to the up, 4 texels to the down
        {
            result += texture(map, TexCoord + vec2(0.0, tex_offset.y * i)) * weight[i];
            result += texture(map, TexCoord - vec2(0.0, tex_offset.y * i)) * weight[i];
        }
    }

    FragColor = result;
}