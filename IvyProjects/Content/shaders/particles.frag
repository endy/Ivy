///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     GLTestApp OpenGL 2.0 Simple Fragment Shader
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

precision mediump float;

varying vec2 v_TexCoord;
uniform sampler2D s_tex0;
uniform sampler2D s_tex1;
varying  vec4 v_Color;

void main(void)
{
    gl_FragColor = vec4(v_TexCoord, 1.0, 1.0); //texture2D(s_tex0, v_TexCoord) + texture2D(s_tex1, v_TexCoord + vec2(0.5, 0.5));
}
