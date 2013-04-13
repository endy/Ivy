///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     DXTestApp HLSL
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


Texture2D <float4> myKitten : register(t0);
SamplerState mySampler : register(s0);

struct VS_PT_OUT
{
    float4 position : SV_POSITION;
    float2 tex0     : TEXCOORD;
};

struct VS_PTN_OUT
{
    float4 position : SV_POSITION;
    float2 tex0     : TEXCOORD;
    float4 normal   : NORMAL;
};

cbuffer MB : register(c0)
{       
    row_major matrix world;
    row_major matrix view;
    row_major matrix projection;
};

cbuffer Light : register(c0)
{
    float4 lposition;
    float4 lcolor;
};

cbuffer Material : register(c1)
{
    float4 diffuse;
    float4 specular;
    float4 ambient;
};


VS_PT_OUT PosTex( float4 Pos : SV_POSITION,
                  float2 Tex : TEXCOORD)
{
    VS_PT_OUT vsOut;

    vsOut.position = mul(Pos, world);
    vsOut.position = mul(vsOut.position, view);
    vsOut.position = mul(vsOut.position, projection);

    vsOut.tex0 = Tex;

    return vsOut;
}


struct VS_P4T3_OUT
{
    float4 position : SV_POSITION;
    float3 tex0     : TEXCOORD;
};

VS_P4T3_OUT InstanceCube( float4 Pos : SV_POSITION,
                          uint id : SV_InstanceID )
{
    VS_P4T3_OUT vsOut;

    vsOut.position = mul(Pos, world);


    int x = -5 + (id % 10);
    int y = -5 + (id / 10)  % 10;
    int z = -5 + (id / 100) % 10;

    float spacing = 0.5;
    float zSpacing = 0.5;

    vsOut.position.x += x;//  + (id%10) * spacing;
    vsOut.position.y += y;// * 0.25  + ((id/10) % 10) * spacing;
    vsOut.position.z += z;// * 0.25  + ((id/100) % 10) * zSpacing;

   // vsOut.tex0 = vsOut.position.xyz;
    vsOut.tex0 = (vsOut.position.xyz + 5.0) / 10.0;

    vsOut.position = mul(vsOut.position, view);
    vsOut.position = mul(vsOut.position, projection);

    return vsOut;
}

float4 ApplyTexture( float4 pos      : SV_POSITION,
                     float2 texCoord : TEXCOORD  )     : SV_TARGET
{
    float4 scale = { -1.0, -1.0, -1.0, 1.0 };
    float4 invert = { 1.0, 1.0, 1.0, 0.0 };
    
    return myKitten.Sample(mySampler, texCoord);
}

VS_PTN_OUT PosTexNorm( float4 Pos : SV_POSITION,
                       float2 Tex : TEXCOORD,
                       float4 Normal : NORMAL      )
{
    VS_PTN_OUT vsOut;

    vsOut.position = mul(Pos, world);
    vsOut.position = mul(vsOut.position, view);
    vsOut.position = mul(vsOut.position, projection);

    vsOut.tex0 = Tex;

    vsOut.normal = mul(Normal, world);
    vsOut.normal = mul(vsOut.normal, view);
    vsOut.normal = mul(vsOut.normal, projection);

    vsOut.normal = normalize(vsOut.normal);

    return vsOut;
}

float4 VisTexCoord( float4 pos      : SV_POSITION,
                    float3 texCoord : TEXCOORD  )     : SV_TARGET
{
    return float4(texCoord, 1.0);
}

float4 VisNormal( float4 pos      : SV_POSITION,
                  float2 texCoord : TEXCOORD,
                  float4 normal   : NORMAL )        : SV_TARGET
{
    return normal; //float4(0.5,0.5,0.5,0.5) + (lcolor * dot(normal, lposition));
}

Texture2D <float4> myDepth : register(t0);
Texture2D <uint4> myStencil : register(t1);

float4 VisDepth( float4 pos : SV_POSITION,
                 float2 dCoord : TEXCOORD0) : SV_TARGET
{
    float4 color = myDepth.Sample(mySampler, dCoord);


    color.g = (myStencil.Load(int3(int2(dCoord*800), 0)).g / 255.0);
    color.a = 1.0;
    return color;
}