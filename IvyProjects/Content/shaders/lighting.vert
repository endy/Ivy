///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     GLTestApp OpenGL 2.0 Simple Lighting Vertex Shader
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

precision highp float;

uniform    mat4 worldMatrix;
uniform    mat4 viewMatrix;
uniform    mat4 projectionMatrix;
attribute  vec3 in_Position;
attribute  vec2 in_TexCoord;
attribute  vec4 in_Normal;
varying    vec4 v_Color;
varying    vec2 v_TexCoord;
varying    vec4 v_Normal;


void main(void)
{
    float scale = 4.0;
    vec4 position = vec4(in_Position, 1.0);
    position = (position * vec4(scale,scale,1,1)) * projectionMatrix * viewMatrix * worldMatrix;
    gl_Position = position;

    v_TexCoord = in_TexCoord;
    v_Normal = (in_Normal * vec4(scale,scale,1,1)) * projectionMatrix * viewMatrix * worldMatrix;
}

