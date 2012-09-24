
///////////////////////////////////////////////////////////////////////////////////////////////////
// DxUI Shader Declarations

Texture2D <float4> colorTex : register(t0);
Texture2D <float4> edgesTex : register(t1);

SamplerState PointSampler : register(s0);
SamplerState LinearSampler : register(s1);


///////////////////////////////////////////////////////////////////////////////////////////////////

struct VS_PT_OUT
{
    float4 position : SV_POSITION;
    float2 tex0     : TEXCOORD;
};

cbuffer MB : register(c0)
{       
    row_major matrix world;
    row_major matrix view;
    row_major matrix projection;
};


VS_PT_OUT PosTex( float4 Pos : SV_POSITION,
                  float2 Tex : TEXCOORD )
{
    VS_PT_OUT vsOut;

    vsOut.position = mul(Pos, world);
    vsOut.position = mul(vsOut.position, view);
    vsOut.position = mul(vsOut.position, projection);

    vsOut.tex0 = Tex;

    return vsOut;
}

VS_PT_OUT PosTexTri( uint VertId : SV_VertexID )
{
    VS_PT_OUT vsOut;

    // Tri 1
    if (VertId == 0)
    {
        vsOut.position = float4(-1, 1, 0.5, 1);
        vsOut.tex0 = float2(0, 0);
    }
    else if (VertId == 1)
    {
        vsOut.position = float4(1, 1, 0.5, 1);
        vsOut.tex0 = float2(1, 0);
    }
    else if (VertId == 2)
    {
        vsOut.position = float4(1, -1, 0.5, 1);
        vsOut.tex0 = float2(1, 1);
    }
    // Tri 2
    else if (VertId == 3)
    {
        vsOut.position = float4(-1, 1, 0.5, 1);
        vsOut.tex0 = float2(0, 0);
    }
    else if (VertId == 4)
    {
        vsOut.position = float4(1, -1, 0.5, 1);
        vsOut.tex0 = float2(1, 1);
    }
    else if (VertId == 5)
    {
        vsOut.position = float4(-1, -1, 0.5, 1);
        vsOut.tex0 = float2(0, 1);
    }

    return vsOut;
}

float4 ApplyTex( float4 pos      : SV_POSITION,
                 float2 texCoord : TEXCOORD )     : SV_TARGET
{
    return colorTex.Sample(PointSampler, texCoord);
}
