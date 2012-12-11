//=============================================================================
// tex.fx by Frank Luna (C) 2008 All Rights Reserved.
//
// Transforms, lights, and textures geometry.
//=============================================================================


#include "lighthelper.fx"
 
 
cbuffer cbPerFrame
{
	Light gLight;
	int gLightType;
	float3 gEyePosW;
};

//bool gSpecularEnabled;

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWVP; 
	float4x4 gTexMtx;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gLayer0;
Texture2D gSpecMap;

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_IN
{
	float3 posL    : POSITION;
	float3 normalL : NORMAL;
	float2 texC    : TEXCOORD;
};

struct VS_OUT
{
	float4 posH    : SV_POSITION;
    float3 posW    : POSITION;
	float shade    : SHADE;
    float3 normalW : NORMAL;
    float2 texC    : TEXCOORD;
};
 
VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	
	// Transform to world space space.
	vOut.posW    = mul(float4(vIn.posL, 1.0f), gWorld);
	vOut.normalW = mul(float4(vIn.normalL, 0.0f), gWorld);

	vOut.shade = saturate(max(dot(vOut.normalW, gEyePosW), 0.0f) );
		
	// Transform to homogeneous clip space.
	vOut.posH = mul(float4(vIn.posL, 1.0f), gWVP);
	
	// Output vertex attributes for interpolation across triangle.
	vOut.texC  = vIn.texC;
	
	return vOut;
}

float4 PS(VS_OUT pIn) : SV_Target
{
	// Get materials from texture maps.
	float4 diffuse = gLayer0.Sample( gTriLinearSam, pIn.texC );
	float4 spec    = gSpecMap.Sample( gTriLinearSam, pIn.texC );
	
	// Map [0,1] --> [0,256]
	spec.a *= 256.0f;
	
	// Interpolating normal can make it not be of unit length so normalize it.
    float3 normalW = normalize(pIn.normalW);
    
	// Compute the lit color for this pixel.
    SurfaceInfo v = {pIn.posW, normalW, diffuse, spec};
	float3 litColor;
	if(gLightType == 0){	//Parallel
		litColor = ParallelLight(v, gLight, gEyePosW,0);
	}
	else if(gLightType == 1){	//Parallel
		litColor = PointLight(v, gLight, gEyePosW,0);
	}
	else if(gLightType == 2){	//Parallel
		litColor = Spotlight(v, gLight, gEyePosW,0);
	}

    return float4(litColor, diffuse.a);
}

technique10 BuildTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
