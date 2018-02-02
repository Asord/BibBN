#version 330 core

in vec3 vertcouleur;
out vec4 color;

void main()
{
    color = vec4(vertcouleur, 1.0f);
}
