#version 330

// Inputs of vertex shaders are set by the application wrt the following locations
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 v_position;

void main()
{
    vec4 worldPosition = proj * view * model * vec4(in_position, 1.0);

    v_position = worldPosition.xyz;
	gl_Position = worldPosition;
}
