#version 330 core
  
layout (location = 0) in vec2 position;

uniform float angle;

void main()
{
    float cosangle = cos(angle);
    float sinangle = sin(angle);
    mat2 rot = mat2(cosangle, sinangle,
                   -sinangle, cosangle);
    gl_Position = vec4(rot * position, 0.0f, 1.0f);
}