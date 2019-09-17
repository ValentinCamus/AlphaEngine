#version 330

// Resulting Fragment Shader
#include "AlphaShaders/Material.glsl"

// Include all the ligh-type interfaces.
#include "AlphaShaders/Light.glsl"

// All pbr function.
#include "AlphaShaders/Pbr/PbrFunction.glsl"

out vec4 fragColor;

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texcoord;
in vec3 v_eye;

// Define the uniform that could be accessed from th application
uniform Material material;

uniform int nLights = 0;
uniform Light lights[MAX_NB_LIGHTS];

// ----------------------------------------------------------------------------
void main()
{
    vec3 albedo     = GetKd(material, v_texcoord);
    vec3 ks         = GetKs(material, v_texcoord);
    vec3 N          = GetNormal(material, v_texcoord);
    float metallic  = GetMetallic(material, v_texcoord);
    float roughness = GetRoughness(material, v_texcoord);
    float ao        = GetAO(material, v_texcoord);
    float trans     = GetTransparency(material, v_texcoord);

    vec3 V = normalize(v_eye - v_position);

    // Calculate reflectance at normal incidence.
    // If dia-electric (like plastic) use F0 of 0.04 and if it's a metal,
    // use the albedo color as F0 (metallic workflow).
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    // Reflectance equation.
    vec3 Lo = vec3(0.0);

    vec3 lightView = vec3(0, 0, 0);

    for(int i = 0; i < nLights; ++i)
    {
        // Calculate per-light radiance.
        vec3 L = GetLightDirection(lights[i], v_position);
        vec3 H = normalize(V + L);

        float attenuation = LightAttenuationFrom(lights[i], v_position);
        vec3 radiance = attenuation * lights[i].color.xyz;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3  F   = FresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3  nominator   = NDF * G * F;
        // Where: 0.001 to prevent divide by zero.
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
        vec3  specular    = nominator / denominator;

        // For energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - (F * ks);

        // Multiply kD by the inverse metalness such that only non-metals
        // have diffuse lighting, or a linear blend if partly metal
        // (pure metals have no diffuse light).
        kD *= 1.0 - metallic;

        // Scale light by NdotL
        float brightness = max(dot(N, L), 0.0);

        // Add to outgoing radiance Lo.
        // Note that we already multiplied the BRDF by the Fresnel (kS),
        // so we won't multiply by kS again.
        Lo += (kD * albedo / Pi + specular) * radiance * brightness;

        lightView += attenuation * brightness;
    }

    // Ambient lighting (note that the next IBL tutorial will replace
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    fragColor = vec4(color, trans);
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2));
}