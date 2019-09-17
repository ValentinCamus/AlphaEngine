#ifndef PLASTICMATERIAL_GLSL
#define PLASTICMATERIAL_GLSL

#include "Pbr/MicrofacetFunction.glsl"

// Material parameters could be textured.
struct MaterialTextures
{
    int hasKd;
    int hasKs;
    int	hasRoughness;
    int hasNormal;
    int hasMetallic;
    int hasAO;
    int hasTransparency;

    sampler2D kd;
    sampler2D ks;
    sampler2D roughness;
    sampler2D normal;
    sampler2D metallic;
    sampler2D ao;
    sampler2D transparency;
};

// Main material description.
struct Material
{

    vec4 kd;
    vec4 ks;
    vec3 normal;

    float roughness;
    float metallic;
    float ao;

    float transparency;

    MaterialTextures tex;
};

vec3 GetKd(Material material, vec2 texCoord)
{
    if (material.tex.hasKd == 0) return material.kd.xyz;
    return vec3(texture(material.tex.kd, texCoord));
}

vec3 GetKs(Material material, vec2 texCoord)
{
    if (material.tex.hasKs == 0) return material.ks.xyz;
    return vec3(texture(material.tex.ks, texCoord));
}

vec3 GetNormal(Material material, vec2 texCoord)
{
    if (material.tex.hasNormal == 0) return material.normal;
    else return vec3(texture(material.tex.normal, texCoord));
}

float GetRoughness(Material material, vec2 texCoord)
{
    if (material.tex.hasRoughness == 0) return material.roughness;
    else return texture(material.tex.roughness, texCoord).r;
}

float GetMetallic(Material material, vec2 texCoord)
{
    if (material.tex.hasMetallic == 0) return material.metallic;
    return texture(material.tex.metallic, texCoord).r;
}

float GetAO(Material material, vec2 texCoord)
{
    if (material.tex.hasAO == 0) return material.ao;
    return texture(material.tex.ao, texCoord).r;
}

float GetTransparency(Material material, vec2 texCoord)
{
    if (material.tex.hasTransparency == 0) return material.transparency;
    return texture(material.tex.transparency, texCoord).r;
}

#endif//PLASTICMATERIAL_GLSL
