
///////////////////////////////////////////////////////////////////////////////////////////////////
// Instancing Declarations

Texture2D <float4> colorTex : register(t0);

SamplerState PointSampler : register(s0);
SamplerState LinearSampler : register(s1);


///////////////////////////////////////////////////////////////////////////////////////////////////

struct VS_PT_OUT
{
    float4 position : SV_POSITION;
    float2 tex0     : TEXCOORD;
};

struct VS_PTN_OUT
{
    float4 position  : SV_POSITION;
    float2 tex0      : TEXCOORD;
    float4 normal    : NORMAL;
    float4 vertColor : COLOR0;
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

VS_PTN_OUT PosTexNorm( float4 Pos : SV_POSITION,
                       float2 Tex : TEXCOORD,
                       float4 Normal : NORMAL      )
{
    VS_PTN_OUT vsOut;

    vsOut.position = mul(Pos, world);
    vsOut.position = mul(vsOut.position, view);
    vsOut.position = mul(vsOut.position, projection);

    vsOut.tex0 = vsOut.position.xy; //Tex;

    Normal = normalize(Normal);

    vsOut.normal = mul(Normal, world);
  //  vsOut.normal = mul(vsOut.normal, view);
  //  vsOut.normal = mul(vsOut.normal, projection);

    vsOut.normal = normalize(Normal); normalize(vsOut.normal);

    vsOut.vertColor = float4(0.4, 0, 0.9, 1);

    return vsOut;
}

float4 ApplyTex( float4 pos      : SV_POSITION,
                     float2 texCoord : TEXCOORD )     : SV_TARGET
{
    return colorTex.Sample(PointSampler, texCoord);
}


float4 CubePS( float4 pos      : SV_POSITION,
               float2 texCoord : TEXCOORD )     : SV_TARGET
{
    float bandLevel = 5.0;

    float4 color = float4(texCoord.x, 0, texCoord.y,1);
    return trunc(color*bandLevel) / bandLevel;
}

