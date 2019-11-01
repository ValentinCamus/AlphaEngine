#version 330

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
    gl_Position = u_proj * u_view * u_model * vec4(in_position, 1.0);

    mat3 normalMatrix = mat3(transpose(inverse(u_view * u_model)));
    vs_out.normal = normalize(vec3(u_proj * vec4(normalMatrix * in_normal, 1.0)));
}

