#version 410 core

out vec4 color;

uniform vec3 u_objColor;
uniform vec3 u_lightColor;

void main()
{
    color = vec4(u_lightColor * u_objColor, 1.0f);
}
