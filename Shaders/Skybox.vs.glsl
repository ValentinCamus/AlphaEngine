#version 330

// Inputs of vertex shaders are set by the application wrt the following locations
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;

out vec3 v_texcoord;

uniform mat4 u_proj;
uniform mat4 u_view;

void main()
{
    v_texcoord = in_position;
    gl_Position = u_proj * u_view * vec4(in_position, 1.0);
}
