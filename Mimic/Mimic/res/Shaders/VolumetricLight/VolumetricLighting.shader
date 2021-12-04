#shader vertex
#version 330 core
layout(Location = 0) in vec2 aPos;
layout(Location = 1) in vec2 aTextureCoord;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
    TexCoords = aTextureCoord;
}







#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 camPos;

const float PI = 3.14159265359;
uniform float g;
uniform int sampleN;

vec3 lightPos = vec3(-70.0f, 70.0f, -10.0f);

uniform sampler2D gPosition;

uniform mat4 lightProjection;
uniform mat4 lightView;
uniform sampler2D shadowMap;


// bayer matrix
const float ditherPattern[16] = float[](0.0f, 0.5f, 0.125f, 0.625f,
    0.75f, 0.22f, 0.875f, 0.375f,
    0.1875f, 0.6875f, 0.0625f, 0.5625,
    0.9375f, 0.4375f, 0.8125f, 0.3125);


float calculateDitherValue(vec2 pixel)
{
    int index_a = int(pixel.x) % 4;
    int index_b = int(pixel.y) % 4;
    return ditherPattern[index_a + index_b * 4];
}


// Mie scaterring approximated with Henyey-Greenstein phase function.
float ComputeScattering(float lightDotView)
{
    float result = 1.0f - g * g;
    result /= (4.0f * PI * pow(1.0f + g * g - (2.0f * g) * lightDotView, 1.5f));
    return result;
}


vec3 calculateVolumetricLighting(vec3 fragPos)
{
    // view Ray
    vec3 viewRay = fragPos - camPos;
    float rayLength = length(viewRay);
    vec3 viewRayDir = viewRay / rayLength;
    // Step length
    float stepLength = rayLength / sampleN;
    //float stepLength = 0.01f;
    vec3 step = viewRayDir * stepLength;	// point from camera to pixel
    // init sampleing position and accumlated fog value
    vec3 currentPos = camPos;
    vec3 accumFog = vec3(0.0);

    // disable dithering
    //if (enableDithering == 1)
    currentPos += step * calculateDitherValue(gl_FragCoord.xy);

    // sampling along with viewRay
    for (int i = 0; i < sampleN; ++i)
    {
        vec4 InLightWorldSpace = lightProjection * lightView * vec4(currentPos, 1.0f);
        vec4 sampleInLightWorldSpace = InLightWorldSpace / InLightWorldSpace.w;

        // transform to [0,1] range
        sampleInLightWorldSpace = sampleInLightWorldSpace * 0.5 + 0.5;
        if (InLightWorldSpace.w <= 0) continue;

        vec4 viewPos = lightView * vec4(currentPos, 1.0f);

        float depthMapDepth = texture(shadowMap, sampleInLightWorldSpace.xy).r;
        if ((depthMapDepth) > sampleInLightWorldSpace.z)
        {
            vec3 sunDir = normalize(lightPos - currentPos);
            accumFog += ComputeScattering(dot(viewRayDir, sunDir)) * vec3(10.0f) / sampleN;
        }

        currentPos += step;	// move to next sample
    }
    return accumFog;
}





void main()
{
    vec3 WorldPos = texture(gPosition, TexCoords * 2).rgb;

    FragColor = vec4(calculateVolumetricLighting(WorldPos), 1.0f);
}
