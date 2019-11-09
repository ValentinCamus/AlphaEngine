#version 330

#include "Alpha/Material.glsl"
#include "Alpha/Light.glsl"
#include "Alpha/Pbr/PbrFunction.glsl"
#include "Alpha/Utils.glsl"

out vec4 fragColor;

in vec3 v_positionInWorldSpace;
in vec4 v_positionInLightSpace;
in vec3 v_normal;
in vec2 v_texcoord;

uniform vec3 u_viewPosition;

uniform Light u_light;
uniform Material u_material;

uniform int u_debug = 0;

// ----------------------------------------------------------------------------
void main()
{
    vec3 kd = GetKd(u_material, v_texcoord);
    vec3 ks = GetKs(u_material, v_texcoord);
    vec3 normal = GetNormal(u_material, v_normal, v_texcoord);
    float metallic = GetMetallic(u_material, v_texcoord);
    float roughness = GetRoughness(u_material, v_texcoord);
    float ao = GetAO(u_material, v_texcoord);
    float trans = GetTransparency(u_material, v_texcoord);

    vec3 viewDirection = normalize(u_viewPosition - v_positionInWorldSpace);
    vec3 lightDirection = GetLightDirection(u_light, v_positionInWorldSpace);
    vec3 halfVector = normalize(viewDirection + lightDirection);

    float nDotV = dot(normal, viewDirection);
    float nDotL = dot(normal, lightDirection);
    float hDotV = dot(halfVector, viewDirection);

    float lightAttenuation = LightAttenuation(u_light, v_positionInWorldSpace);
    vec3 lightRadiance = lightAttenuation * u_light.color.xyz;
    float shadow = GetLightShadow(u_light, v_positionInLightSpace);
    float brightness = max(0, nDotL) * (1 - shadow);

    vec3 lightContribution = lightRadiance * brightness;

    vec3 rgbColor = kd * lightContribution;
    vec4 rgbaColor = vec4(rgbColor, trans);

    fragColor = ApplyGammaCorrection(GAMMA, rgbaColor);
}