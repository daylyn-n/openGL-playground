#version 410 core

in vec3 v_vertexCols;

out vec4 color;

uniform float u_offset;
void main()
{
    color = vec4(v_vertexCols.r + u_offset, v_vertexCols.g, v_vertexCols.b,
            1.0f);
}

