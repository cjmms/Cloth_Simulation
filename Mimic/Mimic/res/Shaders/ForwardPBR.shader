#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
    vec4 FragPosLightSpace;
} vs_out;


uniform mat4 lightProjection;
uniform mat4 lightView;


void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
	vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	vs_out.TexCoord = aTexCoord;
    vs_out.FragPosLightSpace = lightProjection * lightView * model * vec4(aPos, 1.0f);
}





#shader fragment
#version 330 core


in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
    vec4 FragPosLightSpace;
} fs_in;


const int N_LIGHTS = 10;
uniform vec3 lightPositions[N_LIGHTS];
uniform vec3 lightColors[N_LIGHTS];
uniform vec3 camPos;

uniform sampler2D ShadowMap;
uniform int ShadowMapType;

out vec4 FragColor;


vec3 calculateLighting(vec3 normal, int i, vec3 color)
{
	// diffuse
	vec3 lightDir = normalize(lightPositions[i] - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 result = lightColors[i] * diff * color;
	// attenuation (use quadratic as we have gamma correction)
	float distance = length(fs_in.FragPos - lightPositions[i]);
	result *= 1.0 / (distance * distance);
	return result;
}





vec3 solveCramers(vec4 bPrime, vec3 result)
{
    mat3 m0 =
        mat3(1, bPrime.x, bPrime.y,
            bPrime.x, bPrime.y, bPrime.z,
            bPrime.y, bPrime.z, bPrime.w);
    mat3 m1 =
        mat3(result.x, bPrime.x, bPrime.y,
            result.y, bPrime.y, bPrime.z,
            result.z, bPrime.z, bPrime.w);
    mat3 m2 =
        mat3(1, result.x, bPrime.y,
            bPrime.x, result.y, bPrime.z,
            bPrime.y, result.z, bPrime.w);
    mat3 m3 =
        mat3(1, bPrime.x, result.x,
            bPrime.x, bPrime.y, result.y,
            bPrime.y, bPrime.z, result.z);

    float det0 = determinant(m0);
    float det1 = determinant(m1);
    float det2 = determinant(m2);
    float det3 = determinant(m3);

    float x = det1 / det0;
    float y = det2 / det0;
    float z = det3 / det0;

    return vec3(x, y, z);
}


float momentShadowFactor(vec4 shadowSample, float depth)
{
    vec4 bPrime = shadowSample;
    vec3 depthVec = vec3(1.f, depth, depth * depth);

    //-- Calc C. decomp = M, depthVec = D
    vec3 c = solveCramers(bPrime, depthVec);

    //-- Calc roots
    //solve c3 * z^2 + c2 * z + c1 = 0 for z using quadratic formula 
    // -c2 +- sqrt(c2^2 - 4 * c3 * c1) / (2 * c3)
    //Let z2 <= z3 denote solutions 
    float sqrtTerm = sqrt(c.y * c.y - 4 * c.z * c.x);
    float z2 = (-c.y - sqrtTerm) / (2 * c.z);
    float z3 = (-c.y + sqrtTerm) / (2 * c.z);

    if (z2 > z3)
    {
        float temp = z2;
        z2 = z3;
        z3 = temp;
    }

    //-- If depth <= z2
    if (depth <= z2) return 0;

    //-- If depth <= z3
    //  g = (depth * z3 - b'1(depth + z3) + b'2) / ((z3 - z2) * (depth - z2))
    //  return g
    if (depth <= z3)
    {
        return (depth * z3 - bPrime.x * (depth + z3) + bPrime.y) / ((z3 - z2) * (depth - z2));
    }
    //-- if depth > z3
    //  g = (z2 * z3 - b'1(z2 + z3) + b'2) / ((depth - z2) * (depth - z3))
    //  return 1 - g
    else
    {
        return 1 - (z2 * z3 - bPrime.x * (z2 + z3) + bPrime.y) / ((depth - z2) * (depth - z3));
    }
}







vec4 Invalidate(vec4 b)
{
    float alpha = 0.001;
    return (1.0f - alpha) * b + alpha * 30;
}



float calculateShadow(vec4 fragPosLightSpace)
{
    vec2 screenCoords = fragPosLightSpace.xy / fragPosLightSpace.w;
    screenCoords = screenCoords * 0.5 + 0.5; // [0, 1]

    if (fragPosLightSpace.w < 0.0) return 1.0;
    if (screenCoords.x > 1.0 || screenCoords.x < 0.0) return 1.0;
    if (screenCoords.y > 1.0 || screenCoords.y < 0.0) return 1.0;

    float bias = 0.078f;
    // check whether current frag pos is in shadow
    return (fragPosLightSpace.w - bias) > texture(ShadowMap, screenCoords).r ? 0.0 : 1.0;
}



float calculateMSM(vec4 fragPosLightSpace)
{
    // Perspective divide
    vec2 screenCoords = fragPosLightSpace.xy / fragPosLightSpace.w;
    screenCoords = screenCoords * 0.5 + 0.5; // [0, 1]

    if (fragPosLightSpace.w < 0.0) return 1.0;
    if (screenCoords.x > 1.0 || screenCoords.x < 0.0) return 1.0;
    if (screenCoords.y > 1.0 || screenCoords.y < 0.0) return 1.0;

    vec4 b = texture(ShadowMap, screenCoords);

    return 1 - momentShadowFactor(Invalidate(b), fragPosLightSpace.w);
}



// ----------------------------------------------------------------------------
// Variance shadow mapping
float calculateVSM(vec4 fragPosLightSpace) {
    // Perspective divide
    vec2 screenCoords = fragPosLightSpace.xy / fragPosLightSpace.w;
    screenCoords = screenCoords * 0.5 + 0.5; // [0, 1]

    if (fragPosLightSpace.w < 0.0) return 1.0;
    if (screenCoords.x > 1.0 || screenCoords.x < 0.0) return 1.0;
    if (screenCoords.y > 1.0 || screenCoords.y < 0.0) return 1.0;

     float FragLightSpaceDepth = fragPosLightSpace.w; // Use raw distance instead of linear junk
     vec2 moments = texture2D(ShadowMap, screenCoords.xy).rg;

     float mean = moments.x;
     float variance = moments.y - mean * mean;

     if (FragLightSpaceDepth <= mean) return 1.0;    // not under shadow

     // Chebychev inequality
     return variance / (variance + pow(FragLightSpaceDepth - mean, 2.0));
}




void main()
{
    float shadowIntensity;

    if (ShadowMapType == 0) {
        shadowIntensity = calculateShadow(fs_in.FragPosLightSpace);
    }
    else if (ShadowMapType == 1) {
        shadowIntensity = calculateVSM(fs_in.FragPosLightSpace);
    }
    else{
        shadowIntensity = calculateMSM(fs_in.FragPosLightSpace);
    }


	vec3 color = vec3(1.0f);
	vec3 normal = normalize(fs_in.Normal);

	vec3 ambient = 0.05 * color;
	vec3 diffuse = vec3(0.0f);

	for (int i = 0; i < 4; ++i)
	{
		diffuse += calculateLighting(normal, i, color);
	}


    //FragColor = vec4((ambient + diffuse) , 1.0f);   // testing, no shadow

	FragColor = vec4(ambient + diffuse * shadowIntensity, 1.0f);
}