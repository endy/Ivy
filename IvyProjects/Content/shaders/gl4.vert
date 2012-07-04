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
    position = position * worldMatrix;
    position = position * viewMatrix;
    position = position * projectionMatrix;
    //position = position * projectionMatrix * viewMatrix * worldMatrix;

    //position = position * projectionMatrix;
    //position = position * viewMatrix;
    //position = position * worldMatrix;

    gl_Position = position; 
    //gl_Position = vec4(in_Position, 0.25);
    v_Color = in_Color;
    v_Texture = in_Tex;
}

