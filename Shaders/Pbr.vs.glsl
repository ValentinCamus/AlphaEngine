#version 330

// Inputs of vertex shaders are set by the application wrt the following locations
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_texcoord;
out vec3 v_eye;

void main()
{
    vec4 worldPosition = proj * view * model * vec4(in_position, 1.0);

    v_position = worldPosition.xyz;
	v_texcoord = in_texcoord.xy;
	v_normal = (model * vec4(in_normal, 0.0)).xyz;
    v_eye = (inverse(view) * vec4(0, 0, 0, 1)).xyz;

	gl_Position = proj * view * model * vec4(in_position, 1.0);
}
