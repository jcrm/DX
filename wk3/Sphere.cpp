#include "Effects.h"
#include "TextureMgr.h"
#include "InputLayouts.h"
#include "Sphere.h"
 
Sphere::Sphere()
: mRadius(0.0f), mNumSlices(0), mNumStacks(0)
{
}

Sphere::~Sphere()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

void Sphere::init(ID3D10Device* device, float radius, UINT numSlices, UINT numStacks, const InitInfo& initInfo)
{
	md3dDevice = device;
	mVertexType = 2;
	mTech			= fx::SphereFX->GetTechniqueByName("MultiTexTech");
	mfxWVPVar		= fx::SphereFX->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVar		= fx::SphereFX->GetVariableByName("gWorld")->AsMatrix();
	mfxLayer0Var	= fx::SphereFX->GetVariableByName("gLayer0")->AsShaderResource();
	mfxLayer1Var	= fx::SphereFX->GetVariableByName("gLayer1")->AsShaderResource();
	mfxLayer2Var	= fx::SphereFX->GetVariableByName("gLayer2")->AsShaderResource();
	mfxBlendMapVar	= fx::SphereFX->GetVariableByName("gBlendMap")->AsShaderResource();
	mfxSpecMapVar	= fx::SphereFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxEyePosVar	= fx::SphereFX->GetVariableByName("gEyePosW");
	mfxLightVar		= fx::SphereFX->GetVariableByName("gLight");
	mfxLightType	= fx::SphereFX->GetVariableByName("gLightType")->AsScalar();
	
	mInfo = initInfo;

	mRadius    = radius;
	mNumSlices = numSlices;
	mNumStacks = numStacks;

	buildVB();
	buildIB();

	mLayer0   = GetTextureMgr().createTex(initInfo.LayerMapFilename0);
	mLayer1   = GetTextureMgr().createTex(initInfo.LayerMapFilename1);
	mLayer2   = GetTextureMgr().createTex(initInfo.LayerMapFilename2);
	mBlendMap = GetTextureMgr().createTex(initInfo.BlendMapFilename);
	mSpecMap  = GetTextureMgr().createTex(initInfo.SpecMapFilename);
}

void Sphere::draw(D3DXVECTOR3 mEyePos, Light mLights[], int mLightType){
	mfxLayer0Var->SetResource(mLayer0);
	mfxLayer1Var->SetResource(mLayer1);
	mfxLayer2Var->SetResource(mLayer2);
	mfxBlendMapVar->SetResource(mBlendMap);
	mfxSpecMapVar->SetResource(mSpecMap);
	md3dDevice->IASetInputLayout(InputLayout::PosNormalTex);
    md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);   
	GfxObj::draw( mEyePos, mLights, mLightType);
}
void Sphere::buildVB()
{
	VertexPNTList vertices;
	float phiStep = PI/mNumStacks;

	// do not count the poles as rings
	UINT numRings = mNumStacks-1;

	// Compute vertices for each stack ring.
	for(UINT i = 1; i <= numRings; ++i)
	{
		float phi = i*phiStep;

		// vertices of ring
		float thetaStep = 2.0f*PI/mNumSlices;
		for(UINT j = 0; j <= mNumSlices; ++j)
		{
			float theta = j*thetaStep;

			VertexPNT v;

			// spherical to cartesian
			v.pos.x = mRadius*sinf(phi)*cosf(theta);
			v.pos.y = mRadius*cosf(phi);
			v.pos.z = mRadius*sinf(phi)*sinf(theta);

			D3DXVec3Normalize(&v.normal, &v.pos);

			v.texC.x = theta / (2.0f*PI);
			v.texC.y = phi / PI;

			vertices.push_back( v );
		}
	}

	// poles: note that there will be texture coordinate distortion
	vertices.push_back( VertexPNT(0.0f, -mRadius, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f) );
	vertices.push_back( VertexPNT(0.0f, mRadius, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f) );
	
	mNumVertices = (UINT)vertices.size();

	GfxObj::buildVB(vertices);
}
void Sphere::buildIB(){
	IndexList indices;

	UINT northPoleIndex = (UINT)mNumVertices-1;
	UINT southPoleIndex = (UINT)mNumVertices-2;
	UINT numRingVertices = mNumSlices+1;
	// Compute indices for inner stacks (not connected to poles).
	for(UINT i = 0; i < mNumStacks-2; ++i)
	{
		for(UINT j = 0; j < mNumSlices; ++j)
		{
			indices.push_back(i*numRingVertices + j);
			indices.push_back(i*numRingVertices + j+1);
			indices.push_back((i+1)*numRingVertices + j);

			indices.push_back((i+1)*numRingVertices + j);
			indices.push_back(i*numRingVertices + j+1);
			indices.push_back((i+1)*numRingVertices + j+1);
		}
	}

	// Compute indices for top stack.  The top stack was written 
	// first to the vertex buffer.
	for(UINT i = 0; i < mNumSlices; ++i)
	{
		indices.push_back(northPoleIndex);
		indices.push_back(i+1);
		indices.push_back(i);
	}

	// Compute indices for bottom stack.  The bottom stack was written
	// last to the vertex buffer, so we need to offset to the index
	// of first vertex in the last ring.
	// do not count the poles as rings
	UINT baseIndex = (mNumStacks-2)*numRingVertices;
	for(UINT i = 0; i < mNumSlices; ++i)
	{
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex+i);
		indices.push_back(baseIndex+i+1);
	}

	mNumFaces    = (UINT)indices.size()/3;
	mNumIndices  = (UINT)indices.size();

	GfxObj::buildIB(indices);
}
 
