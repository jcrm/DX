#include "GfxObj.h"
#include "Camera.h"
#include "InputLayouts.h"

GfxObj::GfxObj()
: mNumVertices(0),mNumIndices(0), mNumFaces(0), 
md3dDevice(0), mVB(0), mIB(0), scale(1,1,1),pos(0,0,0),
theta(0,0,0), indexed(true), reflected(false), mLayer0(0), mVertexType(0),
mLayer1(0), mLayer2(0), mLayer3(0), mLayer4(0), mBlendMap(0), 
mfxSpecMapVar(0), mSpecMap(0) ,mfxWVPVar(0), mfxWorldVar(0), mTech(0), 
mfxLayer0Var(0), mfxLayer1Var(0),mfxLayer2Var(0), mfxLayer3Var(0), 
mfxLayer4Var(0), mfxBlendMapVar(0),mfxEyePosVar(0), mfxLightVar(0), 
mfxLightType(0){
	D3DXMatrixIdentity(&world);
}

GfxObj::~GfxObj(){
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}
void GfxObj::setTrans(void){
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
void GfxObj::Translate(float x, float y, float z){
	pos.x = x;
	pos.y = y;
	pos.z = z;
	setTrans();
}
void GfxObj::Scale(float x, float y, float z){
	scale.x = x;
	scale.y = y;
	scale.z = z;
	setTrans();
}
D3DXMATRIX GfxObj::GetWorld(){
	return world;
}
// Virtual methods.  Derived client class overrides these methods to 
// implement specific object requirements.

void GfxObj::init (ID3D10Device* device){
	md3dDevice=		device;
}
void GfxObj::buildVB(VertexPNTList& vertices){
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
void GfxObj::buildVB(VertexList& vertices){
	// Create vertex buffer
	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));
}
void GfxObj::buildIB(IndexList& indices){
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
void GfxObj::draw(D3DXVECTOR3 mEyePos, Light mLights[], int mLightType){
	mfxSpecMapVar->SetResource(mSpecMap);
	mfxEyePosVar->SetRawValue(&mEyePos, 0, sizeof(D3DXVECTOR3));
	mfxLightVar->SetRawValue(&mLights[mLightType], 0, sizeof(Light));
	mfxLightType->SetInt(mLightType);
	GfxObj::draw();
}
void GfxObj::draw(){
	setTrans();
	D3DXMATRIX mView = GetCamera().view();
	D3DXMATRIX mProj = GetCamera().proj();
	D3DXMATRIX WVP = world*mView*mProj;
	mfxWVPVar->SetMatrix((float*)&WVP);
	mfxWorldVar->SetMatrix((float*)&world);

	UINT stride = 0;
	switch(mVertexType){
	case 1: stride = sizeof(Vertex); break;
	case 2: stride = sizeof(VertexPNT); break;
	}
	UINT offset = 0;
	D3D10_TECHNIQUE_DESC techDesc;
	mTech->GetDesc( &techDesc );
	for(UINT p = 0; p < techDesc.Passes; ++p){
		mTech->GetPassByIndex( p )->Apply(0);        
		md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
		if(indexed){
			md3dDevice->IASetIndexBuffer( mIB, DXGI_FORMAT_R32_UINT, 0 );
			md3dDevice->DrawIndexed(mNumIndices, 0,0);
		}else{
			md3dDevice->Draw(mNumVertices, 0);
		}
	}
	
}
void GfxObj::drawR(){
	UINT stride = sizeof(VertexPNT);
	UINT offset = 0;
	md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	md3dDevice->DrawIndexed(mNumFaces*3, 0, 0);
}