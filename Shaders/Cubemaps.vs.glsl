#version 330

// Include all the ligh-type interfaces.
#include "Alpha/Light.glsl"

// Inputs of vertex shaders are set by the application wrt the following locations
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;

out vec3 v_normal;
out vec3 v_position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
    v_normal = mat3(transpose(inverse(u_model))) * in_normal;
    v_position = vec3(u_model * vec4(in_position, 1.0));
    gl_Position = u_proj * u_view * u_model * vec4(in_position, 1.0);
}
