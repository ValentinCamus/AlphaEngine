#type vertex
#version 330

// Inputs of vertex shaders are set by the application wrt the following locations
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_texcoord;

// Set of matrices that should be used by the shader
uniform mat4 mvp;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_texcoord;

void main()
{
    vec4 worldPosition = mvp * vec4(in_position, 1.0);

    v_position = worldPosition.xyz;
    v_normal = in_normal;
    v_texcoord = in_texcoord.xy;

    gl_Position = worldPosition;
}

#type fragment
#version 330

out vec4 fragColor;

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texcoord;

void main()
{
//    vec3 kd = GetKd(material, v_texcoord).xyz;
//    vec3 ks = GetKs(material, v_texcoord).xyz;
//    vec3 normal = GetNormal(material, v_texcoord);
//    float roughness = GetRoughness(material, v_texcoord);
//    float transparency = GetTransparency(material, v_texcoord);

    fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}