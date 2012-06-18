///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     GLTestApp OpenGL 4.0 Simple Vertex Shader
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#version 130

uniform    mat4 worldMatrix;
uniform    mat4 viewMatrix;
uniform    mat4 projectionMatrix;
in  vec3 in_Position;
in  vec4 in_Color;
in  vec2 in_Tex;
out vec4 v_Color;
out vec2 v_Texture;

void main(void)
{
    vec4 position = vec4(in_Position, 1.0);
    gl_Position = position * projectionMatrix * viewMatrix * worldMatrix;
    v_Color = in_Color;
    v_Texture = in_Tex;
}

