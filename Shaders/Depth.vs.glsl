#version 330

// Inputs of vertex shaders are set by the application wrt the following locations
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;

uniform mat4 u_lightSpace;
uniform mat4 u_model;

out vec3 v_position;

void main()
{
    gl_Position = u_lightSpace * u_model * vec4(in_position, 1.0);
}
