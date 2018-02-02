#version 330 core

out vec4 color;
uniform vec3 CursorColor;

void main()
{
    color = vec4(CursorColor, 1.0f);
}
