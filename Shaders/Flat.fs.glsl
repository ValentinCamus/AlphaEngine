#version 330 core

out vec4 fragColor;

uniform vec4 u_color = vec4(1, 1, 1, 1);

void main()
{
    fragColor = u_color;
}