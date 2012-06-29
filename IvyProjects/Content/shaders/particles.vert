///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     GLTestApp OpenGL 2.0 Point Primitive Vertex Shader
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

uniform    mat4 worldMatrix;
uniform    mat4 viewMatrix;
uniform    mat4 projectionMatrix;
attribute  vec3 in_Position;
attribute  vec2 in_TexCoord;
varying    vec4 v_Color;
varying    vec2 v_TexCoord;

void main(void)
{
    float scale = 1.0;
    vec4 position = vec4(in_Position, 1.0);
    position = (position * vec4(scale,scale,1,1)) * projectionMatrix * viewMatrix * worldMatrix;
    gl_Position = position; 
    gl_PointSize = 4;

    v_TexCoord = in_TexCoord; //vec2((position.x+3.0)/(2.0*scale), - (position.y+3.0)/(2.0*scale)); //in_TexCoord;
}

