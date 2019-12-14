#version 330

// Include all the ligh-type interfaces.
#include "Alpha/Light.glsl"

const int MAX_BONES = 50;  // max number of bones allowed in a skeleton
const int MAX_WEIGHTS = 3; // max number of bones that can affect a vertex

// Inputs of vertex shaders are set by the application wrt the following locations
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;
layout (location = 3) in ivec3 in_boneIndices;
layout (location = 4) in vec2 in_weights;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

uniform Light u_light;

uniform mat4 u_boneTransforms[MAX_BONES];

out vec3 v_wsPosition;
out vec4 v_lsPosition;
out vec3 v_normal;
out vec2 v_texcoord;

void main()
{
	vec4 localPosition = vec4(0.0);
	vec4 localNormal = vec4(0.0);

	for(int i = 0; i < MAX_WEIGHTS; ++i)
	{
		mat4 boneTransform = u_boneTransforms[in_boneIndices[i]];

		vec4 posePosition = boneTransform * vec4(in_position, 1.0);
		localPosition += posePosition * in_weights[i];

		vec4 worldNormal = boneTransform * vec4(in_normal, 0.0);
		localNormal += worldNormal * in_weights[i];
	}

    vec4 worldPosition = u_proj * u_view * u_model * localPosition;
    vec3 fragPosition = vec3(u_model * localPosition);

    v_wsPosition = worldPosition.xyz;
	v_texcoord = in_texcoord.xy;
	v_normal = localNormal.xyz; // (u_model * localNormal).xyz;
    v_lsPosition = u_light.transform * vec4(fragPosition, 1.0);

	gl_Position = worldPosition;
}
