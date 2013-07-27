///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     DXTestApp HLSL
///
///     Copyright 2012-2013, Brandon Light
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
    float3 worldSpace : POSITION0;
};

struct VS_P4T3_OUT
{
    float4 position : SV_POSITION;
    float3 tex0     : TEXCOORD;
};

cbuffer MB : register(c0)
{       
    row_major matrix world;
    row_major matrix view;
    row_major matrix projection;
};

cbuffer AppConstants
{
    uint screenWidth;
    uint screenHeight;
    uint2 padding;
};

cbuffer Light
{
    float4 lposition;
    float4 lcolor;
};
/*
cbuffer Material : register(c1)
{
    float4 diffuse;
    float4 specular;
    float4 ambient;
};
*/

VS_P4T3_OUT PosTex( float4 Pos : SV_POSITION,
                    float2 Tex : TEXCOORD)
{
    VS_P4T3_OUT vsOut;

    vsOut.position = mul(Pos, world);
    vsOut.position = mul(vsOut.position, view);
    vsOut.position = mul(vsOut.position, projection);

    vsOut.tex0 = float3(Tex, 0.0);

    return vsOut;
}

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

    vsOut.position.x += x;
    vsOut.position.y += y;
    vsOut.position.z += z;

    vsOut.tex0 = (vsOut.position.xyz + 5.0) / 10.0;

    vsOut.position = mul(vsOut.position, view);
    vsOut.position = mul(vsOut.position, projection);

    return vsOut;
}

float4 ApplyTexture( float4 pos      : SV_POSITION,
                     float3 texCoord : TEXCOORD  )     : SV_TARGET
{
    float4 scale = { -1.0, -1.0, -1.0, 1.0 };
    float4 invert = { 1.0, 1.0, 1.0, 0.0 };
    
    return float(texCoord.x); //, 0.0, 1.0); 
    //return myKitten.Sample(mySampler, texCoord.xy);
}

VS_PTN_OUT PosTexNorm( float4 Pos : SV_POSITION,
                       float2 Tex : TEXCOORD,
                       float4 Normal : NORMAL,
                       uint id : SV_InstanceID)
{
    VS_PTN_OUT vsOut;

    vsOut.position = mul(Pos, world);

    int x = -2.5 + (id % 5);
    int z = -2.5 + (id / 5)  % 5;
    int y = 0; //-2.5 + (id / 100) % 10;

    float spacing = 0.5;
    float zSpacing = 0.5;

    vsOut.position.x += x;
    vsOut.position.y += y;
    vsOut.position.z += z;
    
    vsOut.worldSpace = vsOut.position.xyz / vsOut.position.w;

    vsOut.position = mul(vsOut.position, view);
    vsOut.position = mul(vsOut.position, projection);

    vsOut.tex0 = Tex;

    vsOut.normal = mul(Normal, world);

    vsOut.normal = vsOut.normal; //normalize(vsOut.normal);

    return vsOut;
}

float4 VisTexCoord( float4 pos      : SV_POSITION,
                    float3 texCoord : TEXCOORD  )     : SV_TARGET
{
    return float4(texCoord, 1.0);
}


float4 Transparent( float4 pos      : SV_POSITION,
                    float3 texCoord : TEXCOORD  )     : SV_TARGET
{
    return float4(0.5, 0.5, 0.5, 0.25);
}

float4 VisNormal( float4 pos      : SV_POSITION,
                  float2 texCoord : TEXCOORD,
                  float4 normal   : NORMAL,
                  float3 worldSpace : POSITION0)        : SV_TARGET
{
    return normalize(normal);
}

float4 Diffuse(   float4 pos      : SV_POSITION,
                  float2 texCoord : TEXCOORD,
                  float4 normal   : NORMAL,
                  float3 worldSpace : POSITION0)        : SV_TARGET
{
    float3 lightpos = lposition.xyz;
    float3 lightDir = (lightpos.xyz - worldSpace.xyz);
    float distance = length(lightDir);
    lightDir = normalize(lightDir);

    float intensity = saturate(dot(normalize(normal.xyz), lightDir));

    float4 material = float4(1,0,0,1);

    //return normal; //float4(0.5,0.5,0.5,0.5) + 
    return lcolor * intensity;
}

Texture2D <float> myDepth : register(t0);
Texture2D <uint4> myStencil : register(t1);

float4 VisDepth( float4 pos : SV_POSITION,
                 float2 dCoord : TEXCOORD0) : SV_TARGET
{
    float4 color = float4(0,0,0,1);
    color = myDepth.Sample(mySampler, dCoord); //.r * 0.2;

  
    float n = 0.00001;
    float f = 20.0;
    
    float d = (2.0 * n) / (f + n - color.r * (f - n));
    color.rgba = d;

    return color;
}

float4 VisStencil( float4 pos : SV_POSITION,
                   float2 dCoord : TEXCOORD0) : SV_TARGET
{
    float4 color = float4(0,0,0,1);
    color.g = (myStencil.Load(int3(int2(dCoord*uint2(screenWidth, screenHeight)), 0)).g*32 / 255.0);
    
    return color;
}

