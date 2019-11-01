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

    float innerAngle;
    float outerAngle;
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
    return normalize(light.point.position - position);
}

vec3 SpotLightDirection(Light light, vec3 position)
{
    return normalize(light.spot.position - position);
}

float DirectionalLightAttenuation(Light light)
{
    return 1.0f;
}

float PointLightAttenuation(Light light, vec3 position)
{
    float d = length(light.point.position - position);
    float attenuation = light.point.attenuation.constant +
                        light.point.attenuation.linear * d +
                        light.point.attenuation.quadratic * d * d;

    return 1 / attenuation;
}

float SpotLightAttenuation(Light light, vec3 position)
{
    vec3 dir = normalize(light.spot.direction);
    float d = length(light.spot.direction);
    
    float attenuation = light.spot.attenuation.constant +
                        light.spot.attenuation.linear * d +
                        light.spot.attenuation.quadratic * d * d;
                        
    vec3 l = normalize(light.spot.position - position);
    float cosRealAngle = dot(l, dir);
    float cosSpotOuter = cos(light.spot.innerAngle / 2.0);
    float radialAttenuation = pow(clamp((cosRealAngle - cosSpotOuter) /
                                        (1.0 - cosSpotOuter), 0.0, 1.0), 1.6);

    return radialAttenuation / attenuation;
}

float GetLightShadow(Light light, vec4 fragPosLightSpace)
{
    float bias = 0.005;

    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(light.shadowMap, projCoords.xy).r;
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
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
