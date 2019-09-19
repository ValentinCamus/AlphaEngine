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
    vec3 kd = GetKd(material, v_texcoord);
    vec3 ks = GetKs(material, v_texcoord);
    vec3 normal = GetNormal(material, v_normal, v_texcoord);
    float metallic = GetMetallic(material, v_texcoord);
    float roughness = GetRoughness(material, v_texcoord);
    float ao = GetAO(material, v_texcoord);
    float trans = GetTransparency(material, v_texcoord);

    // Calculate reflectance at normal incidence.
    // If dia-electric (like plastic) use f0 of 0.04 and if it's a metal,
    // use the albedo color as f0 (metallic workflow).
    vec3 f0 = mix(vec3(0.04), kd, metallic);
    vec3 viewDirection = normalize(v_eye - v_position);
    vec3 light = vec3(0, 0, 0);
    vec3 lo = vec3(0.0); // Reflectance equation.

    for(int i = 0; i < nLights; ++i)
    {
        // Calculate per-light radiance.
        vec3 lightDirection = GetLightDirection(lights[i], v_position);
        vec3 halfVector = normalize(viewDirection + lightDirection);

        float lightAttenuation = LightAttenuationFrom(lights[i], v_position);
        vec3 lightRadiance = lightAttenuation * lights[i].color.xyz;

        // Cook-Torrance BRDF
        float ndf = DistributionGGX(normal, halfVector, roughness);
        float g = GeometrySmith(normal, viewDirection, lightDirection, roughness);
        vec3  fresnel = FresnelSchlick(max(dot(halfVector, viewDirection), 0.0), f0);

        vec3  nominator = ndf * g * fresnel;
        float nDotV = dot(normal, viewDirection);
        float nDotL = dot(normal, lightDirection);
        float denominator = 4 * max(nDotV, 0.0) * max(nDotL, 0.0);
        vec3  specular = nominator / (denominator + 0.001); // Where: 0.001 to prevent divide by zero.

        // For energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light), to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - (fresnel * ks);

        // Multiply kD by the inverse metalness such that only non-metals
        // have diffuse lighting, or a linear blend if partly metal
        // (pure metals have no diffuse light).
        kD *= 1.0 - metallic;

        // Scale light by NdotL
        float brightness = max(dot(normal, lightDirection), 0.0);

        // Add to outgoing radiance lo.
        // Note that we already multiplied the BRDF by the Fresnel (kS),
        // so we won't multiply by kS again.
        lo += (kD * kd / Pi + specular) * lightRadiance * brightness;

        light += lightAttenuation * brightness;
    }

    // Ambient lighting (note that the next IBL tutorial will replace
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * kd * ao;
    vec3 color = ambient + lo;

    // Gamma correction.
    fragColor = vec4(color, trans);
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2));
}