#version 330 core
out vec4 fragColor;

in vec3 v_normal;
in vec3 v_position;

uniform vec3 u_viewPosition;
uniform samplerCube u_skybox;

void main()
{
    vec3 i = normalize(v_position - u_viewPosition);
    vec3 r = reflect(i, normalize(v_normal));
    fragColor = vec4(texture(u_skybox, r).rgb, 1.0);
}