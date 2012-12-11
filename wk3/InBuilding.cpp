#include "InBuilding.h"
#include "Camera.h"
#include "InputLayouts.h"
#include "TextureMgr.h"

InBuilding::InBuilding()
: mNumVertices(0),mNumIndices(0), mNumFaces(0), 
md3dDevice(0), mVB(0), mIB(0), scale(1,1,1),pos(0,0,0),
theta(0,0,0), mLayer0(0),mLayer1(0), mLayer2(0), 
mfxSpecMapVar(0), mSpecMap(0) ,mfxWVPVar(0), mfxWorldVar(0), 
mTech(0), mfxLayer0Var(0),mfxEyePosVar(0), mfxLightVar(0), 
mfxLightType(0){
	D3DXMatrixIdentity(&world);
}

InBuilding::~InBuilding(){
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}
void InBuilding::setTrans(void){
	D3DXMATRIX m;
	D3DXMatrixIdentity(&world);
	D3DXMatrixScaling(&m, scale.x, scale.y, scale.z);
	world*=m;
	D3DXMatrixRotationY(&m, theta.x);
	world*=m;
	D3DXMatrixRotationY(&m, theta.y);
	world*=m;
	D3DXMatrixRotationY(&m, theta.z);
	world*=m;
	D3DXMatrixTranslation(&m, pos.x, pos.y, pos.z);
	world*=m;
}
void InBuilding::Translate(float x, float y, float z){
	pos.x = x;
	pos.y = y;
	pos.z = z;
	setTrans();
}
void InBuilding::build(float w, float h, float d){
	VertexPNTList vertices;
	VertexPNT v;
	float width = w;
	float height = h;
	float depth = d;
	float thrirdwidth = w/3;
	float twothirdwidth = (2*w)/3;
	float twothirdheight = (2*h)/3;
	//Front
	v.Update(0.1f, 0.1f, 0.1f, 0.0f,  0.0f, -1.0f, 0.0,0.0);
	vertices.push_back(v);
	v.Update(0.1f, height, 0.1f, 0.0f,  0.0f, -1.0f, 0.0,1.0);
	vertices.push_back(v);
	v.Update(thrirdwidth, height, 0.1f, 0.0f,  0.0f, -1.0f, 0.4,1.0);
	vertices.push_back(v);
	v.Update(thrirdwidth, 0.1f, 0.1f, 0.0f,  0.0f, -1.0f, 0.4,0.0);
	vertices.push_back(v);

	v.Update(thrirdwidth, twothirdheight, 0.1f, 0.0f,  0.0f, -1.0f, 0.4,0.7);
	vertices.push_back(v);
	v.Update(twothirdwidth, twothirdheight, 0.1f, 0.0f,  0.0f, -1.0f, 0.8,0.7);
	vertices.push_back(v);

	v.Update(twothirdwidth, height, 0.1f, 0.0f,  0.0f, -1.0f, 0.8,1.0);
	vertices.push_back(v);
	v.Update(width, height, 0.1f, 0.0f,  0.0f, -1.0f, 1.0,1.0);
	vertices.push_back(v);
	v.Update(width, 0.1f, 0.1f, 0.0f,  0.0f, -1.0f, 1.0,0.0);
	vertices.push_back(v);
	v.Update(twothirdwidth, 0.1f, 0.1f, 0.0f,  0.0f, -1.0f, 0.8,0.0);
	vertices.push_back(v);

	//Left Face
	v.Update(0.1f, 0.1f, 0.1f, -1.0f,  0.0f, 0.0f, 0.0,0.0);
	vertices.push_back(v);
	v.Update(0.1f, height, 0.1f, -1.0f,  0.0f, 0.0f, 0.0,1.0);
	vertices.push_back(v);
	v.Update(0.1f, height, depth, -1.0f,  0.0f, 0.0f, 1.0,1.0);
	vertices.push_back(v);
	v.Update(0.1f, 0.1f, depth, -1.0f,  0.0f, 0.0f, 1.0,0.0);
	vertices.push_back(v);

	//Top
	v.Update(0.1f, height, 0.1f, 0.0f,  1.0f, 0.0f, 0.0,0.0);
	vertices.push_back(v);
	v.Update(0.1f, height, depth, 0.0f,  1.0f, 0.0f, 0.0,1.0);
	vertices.push_back(v);
	v.Update(width, height, depth, 0.0f,  1.0f, 0.0f, 1.0,1.0);
	vertices.push_back(v);
	v.Update(width, height, 0.1f, 0.0f,  1.0f, 0.0f, 1.0,0.0);
	vertices.push_back(v);

	//Right
	v.Update(width, 0.1f, 0.1f, 1.0f,  0.0f, 0.0f, 0.0,0.0);
	vertices.push_back(v);
	v.Update(width, 0.1f, depth, 1.0f,  0.0f, 0.0f, 1.0,0.0);
	vertices.push_back(v);
	v.Update(width, height, depth, 1.0f,  0.0f, 0.0f, 1.0,1.0);
	vertices.push_back(v);
	v.Update(width, height, 0.1f, 1.0f,  0.0f, 0.0f, 0.0,1.0);
	vertices.push_back(v);

	//Bottom
	v.Update(0.1f, 0.1f, 0.1f, 0.0f,  -1.0f, 0.0f, 0.0,0.0);
	vertices.push_back(v);
	v.Update(0.1f, 0.1f, depth, 0.0f,  -1.0f, 0.0f, 0.0,1.0);
	vertices.push_back(v);
	v.Update(width, 0.1f, depth, 0.0f,  -1.0f, 0.0f, 1.0,1.0);
	vertices.push_back(v);
	v.Update(width, 0.1f, 0.1f, 0.0f,  -1.0f, 0.0f, 1.0,0.0);
	vertices.push_back(v);

	//Back
	v.Update(width, 0.1f, depth, 0.0f,  0.0f, 1.0f, 0.0,0.0);
	vertices.push_back(v);
	v.Update(width, height, depth, 0.0f,  0.0f, 1.0f, 0.0,1.0);
	vertices.push_back(v);
	v.Update(0.1f, height, depth, 0.0f,  0.0f, 1.0f, 1.0,1.0);
	vertices.push_back(v);
	v.Update(0.1f, 0.1f, depth, 0.0f,  0.0f, 1.0f, 1.0,0.0);
	vertices.push_back(v);

	mNumVertices = (UINT)vertices.size();
	buildVB(vertices);

	IndexList indices;
	//front -l
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(0);
	//front -u
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(6);
	indices.push_back(2);
	indices.push_back(4);
	//front -r
	indices.push_back(9);
	indices.push_back(8);
	indices.push_back(7);
	indices.push_back(7);
	indices.push_back(6);
	indices.push_back(9);
	//left
	indices.push_back(10);
	indices.push_back(11);
	indices.push_back(12);
	indices.push_back(12);
	indices.push_back(13);
	indices.push_back(10);
	//right
	indices.push_back(18);
	indices.push_back(19);
	indices.push_back(16);
	indices.push_back(16);
	indices.push_back(21);
	indices.push_back(18);
	//back
	indices.push_back(26);
	indices.push_back(29);
	indices.push_back(28);
	indices.push_back(28);
	indices.push_back(27);
	indices.push_back(26);
	//top
	indices.push_back(14);
	indices.push_back(17);
	indices.push_back(16);
	indices.push_back(16);
	indices.push_back(15);
	indices.push_back(14);
	//bottom
	indices.push_back(22);
	indices.push_back(23);
	indices.push_back(24);
	indices.push_back(24);
	indices.push_back(25);
	indices.push_back(22);
	

	mNumFaces    = (UINT)indices.size()/3;
	mNumIndices  = (UINT)indices.size();
	buildIB(indices);
}
D3DXMATRIX InBuilding::GetWorld(){
	return world;
}
// Virtual methods.  Derived client class overrides these methods to 
// implement specific object requirements.

void InBuilding::init (ID3D10Device* device, const InitInfo& initInfo, float w, float h, float d){
	md3dDevice=		device;
	mTech			= fx::InBuildingFX->GetTechniqueByName("BuildTech");
	mfxWVPVar		= fx::InBuildingFX->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVar		= fx::InBuildingFX->GetVariableByName("gWorld")->AsMatrix();
	mfxLayer0Var	= fx::InBuildingFX->GetVariableByName("gLayer0")->AsShaderResource();
	mfxSpecMapVar	= fx::InBuildingFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxEyePosVar	= fx::InBuildingFX->GetVariableByName("gEyePosW");
	mfxLightVar		= fx::InBuildingFX->GetVariableByName("gLight");
	mfxLightType	= fx::InBuildingFX->GetVariableByName("gLightType")->AsScalar();

	mInfo = initInfo;

	build(w, h, d);

	mLayer0   = GetTextureMgr().createTex(initInfo.LayerMapFilename0);
	mLayer1   = GetTextureMgr().createTex(initInfo.LayerMapFilename1);
	mLayer2   = GetTextureMgr().createTex(initInfo.LayerMapFilename2);
	mSpecMap  = GetTextureMgr().createTex(initInfo.SpecMapFilename);
}
void InBuilding::buildVB(VertexPNTList& vertices){
	// Create vertex buffer
	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexPNT) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));
}
void InBuilding::buildIB(IndexList& indices){
	// Create index buffer
	D3D10_BUFFER_DESC bufferDesc;
    bufferDesc.Usage           = D3D10_USAGE_DEFAULT;
    bufferDesc.ByteWidth       = sizeof( unsigned int ) * mNumIndices;
    bufferDesc.BindFlags       = D3D10_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags  = 0;
    bufferDesc.MiscFlags       = 0;
    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = &indices[0];
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;
    HR(md3dDevice->CreateBuffer( &bufferDesc, &InitData, &mIB));
}
void InBuilding::draw(D3DXVECTOR3 mEyePos, Light mLights[], int mLightType){

	mfxSpecMapVar->SetResource(mSpecMap);
	md3dDevice->IASetInputLayout(InputLayout::PosNormalTex);
    md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  

	mfxSpecMapVar->SetResource(mSpecMap);
	mfxEyePosVar->SetRawValue(&mEyePos, 0, sizeof(D3DXVECTOR3));
	mfxLightVar->SetRawValue(&mLights[mLightType], 0, sizeof(Light));
	mfxLightType->SetInt(mLightType);

	setTrans();
	D3DXMATRIX mView = GetCamera().view();
	D3DXMATRIX mProj = GetCamera().proj();
	D3DXMATRIX WVP = world*mView*mProj;

	mfxWVPVar->SetMatrix((float*)&WVP);
	mfxWorldVar->SetMatrix((float*)&world);

	UINT stride = sizeof(VertexPNT);
	UINT offset = 0;
	D3D10_TECHNIQUE_DESC techDesc;
	mTech->GetDesc( &techDesc );
	for(UINT p = 0; p < techDesc.Passes; ++p){
		ID3D10EffectPass* pass = mTech->GetPassByIndex( p );
        
		UINT stride = sizeof(VertexPNT);
		UINT offset = 0;

		md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
		md3dDevice->IASetIndexBuffer( mIB, DXGI_FORMAT_R32_UINT, 0 );

		mfxLayer0Var->SetResource(mLayer0);
		pass->Apply(0);
		md3dDevice->DrawIndexed(36, 0,0);

		md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
		md3dDevice->IASetIndexBuffer( mIB, DXGI_FORMAT_R32_UINT, 0 );
		mfxLayer0Var->SetResource(mLayer1);
		pass->Apply(0);
		md3dDevice->DrawIndexed(6, 36,0);

		md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
		md3dDevice->IASetIndexBuffer( mIB, DXGI_FORMAT_R32_UINT, 0 );
		mfxLayer0Var->SetResource(mLayer2);
		pass->Apply(0);
		md3dDevice->DrawIndexed(6, 42,0);
	}
}