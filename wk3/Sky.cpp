#include "Sky.h"
#include "TextureMgr.h"
#include "Camera.h"
#include "Effects.h"
#include "InputLayouts.h"

Sky::Sky(): mCubeMap(0){
}

Sky::~Sky(){
}

void Sky::init(ID3D10Device* device, ID3D10ShaderResourceView* cubemap, float radius){
	md3dDevice = device;
	mCubeMap   = cubemap;
	mVertexType = 1;

	mTech			= fx::SkyFX->GetTechniqueByName("SkyTech");
	mfxWVPVar		= fx::SkyFX->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVar		= fx::SkyFX->GetVariableByName("gWorld")->AsMatrix();
	mfxCubeMapVar	= fx::SkyFX->GetVariableByName("gCubeMap")->AsShaderResource();

	std::vector<D3DXVECTOR3> vertices;
	IndexList indices;

	BuildGeoSphere(2, radius, vertices, indices);

	VertexList skyVerts(vertices.size());
	for(size_t i = 0; i < vertices.size(); ++i){
		// Scale on y-axis to turn into an ellipsoid to make a flatter Sky surface
		skyVerts[i].pos = 0.5f*vertices[i];
	}
	mNumVertices = skyVerts.size();
	GfxObj::buildVB(skyVerts);

	mNumIndices = (UINT)indices.size();
	mNumFaces = mNumIndices*3;

	GfxObj::buildIB(indices);
}

void Sky::draw(){
	D3DXVECTOR3 eyePos = GetCamera().position();

	// center Sky about eye in world space
	Translate(eyePos.x, eyePos.y, eyePos.z);
	HR(mfxCubeMapVar->SetResource(mCubeMap));
	md3dDevice->IASetInputLayout(InputLayout::Pos);
	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	GfxObj::draw();
}
 