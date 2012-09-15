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
varying vec4 v_Normal;
uniform sampler2D s_tex0;
uniform sampler2D s_tex1;
varying  vec4 v_Color;

uniform vec4 lightPos;

void main(void)
{
    float diffuseMat = 0.7;
    float ambientMat = 0.0;

    vec4 color = texture2D(s_tex0, v_TexCoord);
   // color = vec4(1.0, 0.0, 0.0, 1.0);

    // N * L
    vec4 nLightPos = normalize(lightPos * vec4(1,1,-1,1));
    vec4 normalOrig = texture2D(s_tex1, v_TexCoord);
    vec4 nNormal = normalize((2.0 * normalOrig)  - 1.0);

    float diff = dot(nLightPos, nNormal);
    if (diff <= 0.0) 
    { 
        diff = 0.0;
    }
    vec4 diffuse = diffuseMat * diff * color;
    vec4 ambient = ambientMat * color;

    gl_FragColor = diffuse; // + ambient;

    //gl_FragColor = texture2D(s_tex0, v_TexCoord) + texture2D(s_tex1, v_TexCoord + vec2(0.5, 0.5));
}
