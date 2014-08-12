///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     GLTestApp OpenGL 4.0 Text Rendering Vertex Shader
///
///     Copyright 2014, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#version 130

uniform    vec2 charPos;
in  vec3 in_Position;
in  vec4 in_Color;
in  vec2 in_Tex;
out vec4 v_Color;
out vec2 v_Texture;

void main(void)
{
    vec4 position = vec4(in_Position, 1.0);

  
    gl_Position = position; 
    gl_Position = vec4(in_Position.xy-vec2(1.0, 1.0), 0, 1.0);
    v_Color = in_Color;
    v_Texture = vec2((in_Position.x*256.0)/512.0, (in_Position.y*256.0)/512.0 + 128/256.0);
}

