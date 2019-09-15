#type vertex
#version 330

// Inputs of vertex shaders are set by the application wrt the following locations
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;

// Set of matrices that should be used by the shader
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_texcoord;

void main()
{
    vec4 worldPosition = proj * view * model * vec4(in_position, 1.0);

    v_position = worldPosition.xyz;
    v_normal = in_normal;
    v_texcoord = in_texcoord.xy;

    gl_Position = worldPosition;
}

#type fragment
#version 330

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
    if (material.tex.hasKd == 0) return material.kd.xyz;
    else return vec3(texture(material.tex.kd, texCoord));
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

out vec4 fragColor;

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texcoord;

uniform Material material;

void main()
{
    vec3 kd = GetKd(material, v_texcoord).xyz;
//    vec3 ks = GetKs(material, v_texcoord).xyz;
//    vec3 normal = GetNormal(material, v_texcoord);
//    float roughness = GetRoughness(material, v_texcoord);
//    float transparency = GetTransparency(material, v_texcoord);

    fragColor = vec4(kd, 1.0f);
}