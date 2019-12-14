#version 330

// Include all the ligh-type interfaces.
#include "Alpha/Light.glsl"

// Inputs of vertex shaders are set by the application wrt the following locations
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

uniform Light u_light;

out vec3 v_wsPosition;
out vec4 v_lsPosition;
out vec3 v_normal;
out vec2 v_texcoord;

void main()
{
    vec4 worldPosition = u_proj * u_view * u_model * vec4(in_position, 1.0);
    vec3 fragPosition = vec3(u_model * vec4(in_position, 1.0));

    v_wsPosition = worldPosition.xyz;
	v_texcoord = in_texcoord.xy;
	v_normal = (u_model * vec4(in_normal, 0.0)).xyz;
    v_lsPosition = u_light.transform * vec4(fragPosition, 1.0);

	gl_Position = worldPosition;
}
