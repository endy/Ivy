///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     GLTestApp OpenGL 4.0 Simple Fragment Shader
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#version 130

uniform sampler2D s_texture;

in  vec4 v_Color;
in  vec2 v_Texture;
out vec4 out_Color;

void main(void)
{
    out_Color = texture(s_texture, v_Texture);
   // out_Color = vec4(v_Texture, 0, 1);
}
