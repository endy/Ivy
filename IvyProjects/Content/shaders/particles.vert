///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     GLTestApp OpenGL 2.0 Simple Vertex Shader
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

uniform    mat4 worldMatrix;
uniform    mat4 viewMatrix;
uniform    mat4 projectionMatrix;
attribute  vec4 in_Position;
attribute  vec4 in_Color;
attribute  vec2 in_TexCoord;
varying    vec4 v_Color;
varying    vec2 v_TexCoord;

void main(void)
{
   // vec4 position = vec4(in_Position, 1.0);
    vec4 position = (in_Position * vec4(4,4,1,1)) * projectionMatrix * viewMatrix * worldMatrix;
    gl_Position = position; //in_Position;
    gl_PointSize = 4;

    v_TexCoord = vec2((in_Position.x+1.0)/2.0, -(in_Position.y+1.0)/2.0); //in_TexCoord;
    v_Color = in_Position; //in_Color;
}

