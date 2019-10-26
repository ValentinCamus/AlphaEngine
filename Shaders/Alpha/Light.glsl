#ifndef DEFAULTLIGHTSHADER_GLSL
#define DEFAULTLIGHTSHADER_GLSL

#define MAX_NB_LIGHTS 32

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

vec3 LightContributionFrom(Light light, vec3 position)
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

float LightAttenuationFrom(Light light, vec3 position)
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
