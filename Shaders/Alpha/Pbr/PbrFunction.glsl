#ifndef PBRFUNCTION_GLSL
#define PBRFUNCTION_GLSL

#include "MicrofacetFunction.glsl"

// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = Pi * denom * denom;

    return nom / denom;
}

// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float nDotV = max(dot(N, V), 0.0);
    float nDotL = max(dot(N, L), 0.0);
    float ggx1  = GeometrySchlickGGX(nDotV, roughness);
    float ggx2  = GeometrySchlickGGX(nDotL, roughness);

    return ggx1 * ggx2;
}

// ----------------------------------------------------------------------------
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// ----------------------------------------------------------------------------
vec3 GetReflectanceAtNormalIncidence(vec3 kd, float metallic)
{
    // If dia-electric (like plastic) use f0 of 0.04 and if it's a metal,
    // use the albedo color as f0 (metallic workflow).
    return mix(vec3(0.04), kd, metallic);
}

#endif // PBRFUNCTION_GLSL