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

uniform bool u_debug = false;

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

    // Light contribution
    vec3 lightRadiance = LightContribution(u_light, v_positionInWorldSpace);
    float shadow = GetLightShadow(u_light, v_positionInLightSpace);
    float brightness = max(0, nDotL) * (1 - shadow);

    vec3 lightContribution = lightRadiance * brightness;

    // Cook-Torrance BRDF
    vec3 f0 = GetReflectanceAtNormalIncidence(kd, metallic);

    float d = DistributionGGX(normal, halfVector, roughness);
    float g = GeometrySmith(normal, viewDirection, lightDirection, roughness);
    vec3  f = FresnelSchlick(max(hDotV, 0.0), f0);
    vec3  specular = d * g * f / max(4 * max(nDotV, 0.0) * max(nDotL, 0.0), 0.001);

    // For energy conservation, the diffuse and specular u_light can't
    // be above 1.0 (unless the surface emits u_light), to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    //
    // Multiply kD by the inverse metalness such that only non-metals
    // have diffuse lighting, or a linear blend if partly metal
    // (pure metals have no diffuse light).
    vec3 kS = f * ks;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

    // Note that we already multiplied the BRDF by the Fresnel (kS),
    // so we won't multiply by kS again.
    vec3 outgoingRadiance = (kD * kd / Pi + specular);
    vec3 ambient = vec3(0.03) * kd * ao; // Ambient lighting.

    vec3 rgbColor = outgoingRadiance * lightContribution + ambient;
    vec4 rgbaColor = vec4(rgbColor, trans);

    fragColor = ApplyGammaCorrection(GAMMA, rgbaColor);
}