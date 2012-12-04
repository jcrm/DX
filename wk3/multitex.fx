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

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWVP; 
	float4x4 gTexMtx;
};


// Nonnumeric values cannot be added to a cbuffer.
Texture2D gLayer0;
Texture2D gLayer1;
Texture2D gLayer2;
Texture2D gBlendMap;
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
	float4 posH			: SV_POSITION;
	float shade         : SHADE;
    float3 posW			: POSITION;
    float4 normalW		: NORMAL;
    float2 tiledUV      : TEXCOORD0;
    float2 stretchedUV  : TEXCOORD1; 

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
		
	vOut.tiledUV     = 16*vIn.texC;
	vOut.stretchedUV = vIn.texC;

	
	return vOut;
}

float4 PS(VS_OUT pIn) : SV_Target
{
	float4 diffuse = (0.0f,0.0f,0.0f,1.0f);
    float4 spec    = gSpecMap.Sample( gTriLinearSam, pIn.tiledUV );	
	// Map [0,1] --> [0,256]
	spec.a *= 256.0f;  

    // Interpolating normal can make it not be of unit length so normalize it.
    float3 normalW = normalize(pIn.normalW);

	// Get materials from texture maps for diffuse col.
		
	float4 c1 = gLayer0.Sample( gTriLinearSam, pIn.tiledUV );
	float4 c2 = gLayer1.Sample( gTriLinearSam, pIn.tiledUV );
	float4 c3 = gLayer2.Sample( gTriLinearSam, pIn.tiledUV );
	
	float4 t = gBlendMap.Sample( gTriLinearSam, pIn.stretchedUV ); 
	
	// Find the inverse of all the blend weights so that we can  scale the total color to the range [0, 1].
    float totalInverse = 1.0f / (t.r + t.g + t.b);
    
    // Scale the colors by each layer by its corresponding weight
    // stored in the blendmap.  
    c1 *= t.r * totalInverse;
    c2 *= t.g * totalInverse;
	c3 *= t.b * totalInverse;
	diffuse = c1+c2+c3;
	// Compute the lit color for this pixel.
    SurfaceInfo v = {pIn.posW, normalW, diffuse, spec};

	float3 litColor;
	if(gLightType == 0){	//Parallel
		litColor = ParallelLight(v, gLight, gEyePosW, 0);
	}
	else if(gLightType == 1){	//Paralle
		litColor = PointLight(v, gLight, gEyePosW, 0);
	}
	else if(gLightType == 2){	//Parallel
		litColor = Spotlight(v, gLight, gEyePosW, 0);
	}
	//shade*diffuse component
	return float4(litColor,diffuse.a);
}

technique10 MultiTexTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
