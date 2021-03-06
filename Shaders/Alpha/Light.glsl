#ifndef DEFAULTLIGHTSHADER_GLSL
#define DEFAULTLIGHTSHADER_GLSL

#define MAX_NB_LIGHTS 4

struct Attenuation
{
    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight
{
    vec3 direction;
};

struct PointLight
{
    vec3 position;

    Attenuation attenuation;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;

    Attenuation attenuation;

    float cutOff;
};

struct Light
{
    int type;
    vec4 color;
    mat4 transform;

    sampler2D shadowMap;

    DirectionalLight directional;
    PointLight point;
    SpotLight spot;
};

vec3 DirectionalLightDirection(Light light)
{
    return -light.directional.direction;
}

vec3 PointLightDirection(Light light, vec3 position)
{
    return -normalize(light.point.position - position);
}

vec3 SpotLightDirection(Light light, vec3 position)
{
    return -normalize(light.spot.position - position);
}

float DirectionalLightAttenuation(Light light)
{
    return 1.0f;
}

float PointLightAttenuation(Light light, vec3 position)
{
    float distance = length(light.point.position - position);
    float attenuation = 1.0 / (light.point.attenuation.constant +
                               light.point.attenuation.linear * distance +
                               light.point.attenuation.quadratic * pow(distance, 2));
    return attenuation;
}

float SpotLightAttenuation(Light light, vec3 position)
{
    vec3 lightDir = SpotLightDirection(light, position);
    // Check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-light.spot.direction));
    if(theta > light.spot.cutOff)
    {
        // attenuation
        float distance = length(light.spot.position - position);
        float attenuation = 1.0 / (light.spot.attenuation.constant +
                                   light.spot.attenuation.linear * distance +
                                   light.spot.attenuation.quadratic * pow(distance, 2));

        return attenuation;
    }
    else
    {
        return 0.1; // Ambient lighting
    }
}

float DirectionalLightShadow(Light light, vec4 positionLightSpace)
{
    float bias = 0.005;

    // Perform perspective divide
    vec3 projCoords = positionLightSpace.xyz / positionLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(light.shadowMap, projCoords.xy).r;
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(light.shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            vec2 coord = projCoords.xy + vec2(x, y) * texelSize;
            float pcfDepth = texture(light.shadowMap, coord).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if (projCoords.z > 1.0) shadow = 0.0;

    return shadow;
}

float PointLightShadow(Light light, vec4 positionLightSpace)
{
    return 0.0f;
}

float SpotLightShadow(Light light, vec4 positionLightSpace)
{
    float bias = 0.005;

    // Perform perspective divide
    vec3 projCoords = positionLightSpace.xyz / positionLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(light.shadowMap, projCoords.xy).r;
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(light.shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            vec2 coord = projCoords.xy + vec2(x, y) * texelSize;
            float pcfDepth = texture(light.shadowMap, coord).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if (projCoords.z > 1.0) shadow = 0.0;

    return shadow;
}

float GetLightShadow(Light light, vec4 positionLightSpace)
{
     switch (light.type)
     {
        case 0: return DirectionalLightShadow(light, positionLightSpace);
        case 1: return PointLightShadow(light, positionLightSpace);
        case 2: return SpotLightShadow(light, positionLightSpace);
        default: break;
    }
    return 0.0f;
}

vec3 GetLightDirection(Light light, vec3 position)
{
     switch (light.type)
     {
        case 0: return DirectionalLightDirection(light);
        case 1: return PointLightDirection(light, position);
        case 2: return SpotLightDirection(light, position);
        default: break;
    }
    return vec3(0.0f);
}

vec3 LightContribution(Light light, vec3 position)
{
    switch (light.type)
    {
        case 0: return DirectionalLightAttenuation(light) * light.color.xyz;
        case 1: return PointLightAttenuation(light, position) * light.color.xyz;
        case 2: return SpotLightAttenuation(light, position) * light.color.xyz;
        default: break;
    }
    return vec3(0.0f);
}

float LightAttenuation(Light light, vec3 position)
{
    switch (light.type)
    {
        case 0:  return DirectionalLightAttenuation(light);
        case 1:  return PointLightAttenuation(light, position);
        case 2:  return SpotLightAttenuation(light, position);
        default: break;
    }
    return 0;
}

#endif //DEFAULTLIGHTSHADER_GLSL
