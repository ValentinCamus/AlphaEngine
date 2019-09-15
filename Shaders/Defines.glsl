#version 330

#ifndef __CORE_DEFINES__
#define __CORE_DEFINES__

struct Transform
{
    mat4 model; // The model matrix (based on the Location-Rotation-Scale vector)
    mat4 view;  // The camera/view matrix (based on the Location-Rotation-Scale vector)
    mat4 proj;  // The projection matrix
    mat4 mvp;   // Projection * View * Projection
};

// Material parameters could be textured.
struct MaterialTextures
{
    int hasKd;
    int hasKs;
    int hasNormal;
    int	hasRoughness;
    int hasTransparency;

    sampler2D kd;
    sampler2D ks;
    sampler2D normal;
    sampler2D roughness;
    sampler2D transparency;
};

// Main material description.
struct Material
{
    vec4 kd;
    vec4 ks;
    vec3 normal;
    float roughness;
    float transparency;

    MaterialTextures tex;
};

vec3 GetKd(Material material, vec2 texCoord)
{
    if (material.tex.hasKd == 1)
    {
        return vec3(texture(material.tex.kd, texCoord));
    }
    return material.kd.xyz;
}

vec3 GetKs(Material material, vec2 texCoord)
{
    if (material.tex.hasKs == 0) return material.ks.xyz;
    else return vec3(texture(material.tex.ks, texCoord));
}

float GetRoughness(Material material, vec2 texCoord)
{
    if (material.tex.hasRoughness == 0) return material.roughness;
    else return texture(material.tex.roughness, texCoord).r;
}

vec3 GetNormal(Material material, vec2 texCoord, vec3 N, vec3 T, vec3 B)
{
    if (material.tex.hasNormal == 0) return material.normal;
    else return texture(material.tex.normal, texCoord).xyz;
}

float GetTransparency(Material material, vec2 texCoord)
{
    if (material.tex.hasTransparency == 1) return material.transparency;
    else return texture(material.tex.transparency, texCoord).r;
}

#endif


