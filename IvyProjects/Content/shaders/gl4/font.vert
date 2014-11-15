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
in  vec4 in_Position;
in  vec4 in_Color;
in  vec2 in_Tex;
out vec4 v_Color;
out vec2 v_Texture;

void main(void)
{
    vec4 position = vec4(in_Position.xyz, 1.0);

	float cellWidth = 32;
	float cellHeight = 32;
	float charWidth = 15;
	float charHeight = 20;
	float texWidth = 256;
	float texHeight = 256;

  
    gl_Position = position; 
    gl_Position = vec4(in_Position.xy-vec2(1.0, 1.0), 0, 1.0);
    v_Color = in_Color;
    // v_Texture = vec2((in_Position.x*256.0)/512.0, (in_Position.y*256.0)/512.0 + 128/256.0);
	v_Texture = vec2((in_Position.z*charWidth)/texWidth, (in_Position.w * charHeight)/texHeight);

	float col = float(int(in_Position.z));
	float row = float(int(in_Position.w));

	if (in_Position.w - row < 0.10)
	{
		// TL
		v_Texture = vec2((col*cellWidth)/texWidth, ((row * cellHeight)+charHeight)/texHeight);
	}
	else if (in_Position.w - row < 0.20)
	{
		// TR
		v_Texture = vec2(((col*cellWidth)+charWidth)/texWidth, ((row * cellHeight)+charHeight)/texHeight);
	}
	else if (in_Position.w - row < 0.30)
	{
		// BR
		v_Texture = vec2(((col*cellWidth)+charWidth)/texWidth, ((row * cellHeight))/texHeight);
	}
	else if (in_Position.w - row < 0.40)
	{
		// BL
		v_Texture = vec2((col*cellWidth)/texWidth, ((row * cellHeight))/texHeight);
	}
}

