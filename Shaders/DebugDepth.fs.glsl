#version 330 core

out vec4 fragColor;

in vec2 v_texcoord;

uniform sampler2D u_depthMap;

void main()
{
    float depthValue = texture(u_depthMap, v_texcoord).r;
    fragColor = vec4(vec3(depthValue), 1.0);
}