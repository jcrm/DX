#include "Effects.h"
#include "TextureMgr.h"
#include "InputLayouts.h"
#include "Cube.h"

Cube::Cube(){

}
Cube::~Cube(){

}
void Cube::initR(ID3D10Device* device){
	md3dDevice		= device;
	mVertexType		= 2;
	mNumVertices	= 8;
	mNumFaces		= 12;
	mNumIndices		= 36;

	reflected = true;
	
	buildVB();
	buildIB();
}
void Cube::init(ID3D10Device* device){
	md3dDevice = device;
	mVertexType= 2;

	mNumVertices=	8;
	mNumFaces=		12;
	mNumIndices =	36;

	buildVB();
	buildIB();
}

void Cube::init(ID3D10Device* device, const InitInfo& initInfo){
	md3dDevice = device;
	mVertexType= 2;
	
	CreateFX();	

	mInfo = initInfo;

	mNumVertices=	8;
	mNumFaces=		12;
	mNumIndices =	36;

	buildVB();
	buildIB();

	mLayer0   = GetTextureMgr().createTex(initInfo.LayerMapFilename0);
	mLayer1   = GetTextureMgr().createTex(initInfo.LayerMapFilename1);
	mLayer2   = GetTextureMgr().createTex(initInfo.LayerMapFilename2);
	mBlendMap = GetTextureMgr().createTex(initInfo.BlendMapFilename);
	mSpecMap  = GetTextureMgr().createTex(initInfo.SpecMapFilename);
}
void Cube::CreateFX(){
	mTech			= fx::MultiCubeFX->GetTechniqueByName("MultiTexTech");
	mfxWVPVar		= fx::MultiCubeFX->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVar		= fx::MultiCubeFX->GetVariableByName("gWorld")->AsMatrix();
	mfxLayer0Var	= fx::MultiCubeFX->GetVariableByName("gLayer0")->AsShaderResource();
	mfxLayer1Var	= fx::MultiCubeFX->GetVariableByName("gLayer1")->AsShaderResource();
	mfxLayer2Var	= fx::MultiCubeFX->GetVariableByName("gLayer2")->AsShaderResource();
	mfxBlendMapVar	= fx::MultiCubeFX->GetVariableByName("gBlendMap")->AsShaderResource();
	mfxSpecMapVar	= fx::MultiCubeFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxEyePosVar	= fx::MultiCubeFX->GetVariableByName("gEyePosW");
	mfxLightVar		= fx::MultiCubeFX->GetVariableByName("gLight");
	mfxLightType	= fx::MultiCubeFX->GetVariableByName("gLightType")->AsScalar();
}
void Cube::buildVB(){
	VertexPNTList vertices;
	VertexPNT v;
	//Front
	v.Update(-1.0f, -1.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0,0.0);
	vertices.push_back(v);
	v.Update(-1.0f, +1.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0,1.0);
	vertices.push_back(v);
	v.Update(+1.0f, +1.0f, -1.0f, 0.0f,  0.0f, -1.0f,1.0,1.0);
	vertices.push_back(v);
	v.Update(+1.0f, -1.0f, -1.0f, 0.0f,  0.0f, -1.0f,1.0,0.0);
	vertices.push_back(v);
	//Left Face
	v.Update(-1.0f, -1.0f, +1.0f,-1.0f,  0.0f, 0.0f,0.0,0.0);
	vertices.push_back(v);
	v.Update(-1.0f, +1.0f, +1.0f,-1.0f,  0.0f, 0.0f,0.0,1.0);
	vertices.push_back(v);
	v.Update(-1.0f, +1.0f, -1.0f,-1.0f,  0.0f, 0.0f,1.0,1.0);
	vertices.push_back(v);
	v.Update(-1.0f, -1.0f, -1.0f,-1.0f,  0.0f, 0.0f,1.0,0.0);
	vertices.push_back(v);

	//Top
	v.Update(-1.0f, +1.0f, -1.0f, 0.0f,  1.0f, 0.0f,0.0,0.0);
	vertices.push_back(v);
	v.Update(-1.0f, +1.0f, +1.0f, 0.0f,  1.0f, 0.0f,0.0,1.0);
	vertices.push_back(v);
	v.Update(+1.0f, +1.0f, +1.0f, 0.0f,  1.0f, 0.0f,1.0,1.0);
	vertices.push_back(v);
	v.Update(+1.0f, +1.0f, -1.0f, 0.0f,  1.0f, 0.0f,1.0,0.0);
	vertices.push_back(v);

	//Right
	v.Update(+1.0f, -1.0f, -1.0f, 1.0f,  0.0f, 0.0f,0.0,0.0);
	vertices.push_back(v);
	v.Update(+1.0f, +1.0f, -1.0f, 1.0f,  0.0f, 0.0f,0.0,1.0);
	vertices.push_back(v);
	v.Update(+1.0f, +1.0f, +1.0f, 1.0f,  0.0f, 0.0f,1.0,1.0);
	vertices.push_back(v);
	v.Update(+1.0f, -1.0f, +1.0f, 1.0f,  0.0f, 0.0f,1.0,0.0);
	vertices.push_back(v);

	//Bottom
	v.Update(-1.0f, -1.0f, +1.0f, 0.0f,  -1.0f, 0.0f,0.0,0.0);
	vertices.push_back(v);
	v.Update(-1.0f, -1.0f, -1.0f, 0.0f,  -1.0f, 0.0f,0.0,1.0);
	vertices.push_back(v);
	v.Update(+1.0f, -1.0f, -1.0f, 0.0f,  -1.0f, 0.0f,1.0,1.0);
	vertices.push_back(v);
	v.Update(+1.0f, -1.0f, +1.0f, 0.0f,  -1.0f, 0.0f,1.0,0.0);
	vertices.push_back(v);

	//Back
	v.Update(+1.0f, -1.0f, +1.0f, 0.0f,  0.0f, 1.0f,0.0,0.0);
	vertices.push_back(v);
	v.Update(+1.0f, +1.0f, +1.0f, 0.0f,  0.0f, 1.0f,0.0,1.0);
	vertices.push_back(v);
	v.Update(-1.0f, +1.0f, +1.0f, 0.0f,  0.0f, 1.0f,1.0,1.0);
	vertices.push_back(v);
	v.Update(-1.0f, -1.0f, +1.0f, 0.0f,  0.0f, 1.0f,1.0,0.0);
	vertices.push_back(v);

	mNumVertices = (UINT)vertices.size();

	GfxObj::buildVB(vertices);
}
void Cube::buildIB(){
	IndexList indices;

	for(int i = 0; i <6;i++){
		indices.push_back(0+(4*i));
		indices.push_back(1+(4*i));
		indices.push_back(2+(4*i));
		indices.push_back(2+(4*i));
		indices.push_back(3+(4*i));
		indices.push_back(0+(4*i));
	}
	mNumFaces    = (UINT)indices.size()/3;
	mNumIndices  = (UINT)indices.size();

	GfxObj::buildIB(indices);
}
void Cube::draw(){
	if(reflected){
		GfxObj::drawR();
	}else{
		GfxObj::draw();
	}
}
void Cube::draw(D3DXVECTOR3 mEyePos, Light mLights[], int mLightType){
	mfxLayer0Var->SetResource(mLayer0);
	mfxLayer1Var->SetResource(mLayer1);
	mfxLayer2Var->SetResource(mLayer2);
	mfxBlendMapVar->SetResource(mBlendMap);
	mfxSpecMapVar->SetResource(mSpecMap);
	md3dDevice->IASetInputLayout(InputLayout::PosNormalTex);
    md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);   
	GfxObj::draw(mEyePos, mLights, mLightType);
}


/*
		{D3DXVECTOR3(-1.0f, -1.0f, -1.0f)},		//A
		{D3DXVECTOR3(-1.0f, +1.0f, -1.0f)},		//B
		{D3DXVECTOR3(+1.0f, +1.0f, -1.0f)},		//C
		{D3DXVECTOR3(+1.0f, +1.0f, -1.0f)},		//C
		{D3DXVECTOR3(+1.0f, -1.0f, -1.0f)},		//D
		{D3DXVECTOR3(-1.0f, -1.0f, -1.0f)},		//A

		{D3DXVECTOR3(-1.0f, -1.0f, +1.0f)},		//E
		{D3DXVECTOR3(-1.0f, +1.0f, +1.0f)},		//F
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f)},		//G
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f)},		//G
		{D3DXVECTOR3(+1.0f, -1.0f, +1.0f)},		//H
		{D3DXVECTOR3(-1.0f, -1.0f, +1.0f)},		//E
		
		{D3DXVECTOR3(+1.0f, -1.0f, -1.0f)},		//D
		{D3DXVECTOR3(+1.0f, +1.0f, -1.0f)},		//C
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f)},		//G
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f)},		//G
		{D3DXVECTOR3(+1.0f, -1.0f, +1.0f)},		//H
		{D3DXVECTOR3(+1.0f, -1.0f, -1.0f)},		//D

		{D3DXVECTOR3(-1.0f, -1.0f, +1.0f)},		//E
		{D3DXVECTOR3(-1.0f, -1.0f, -1.0f)},		//A
		{D3DXVECTOR3(+1.0f, -1.0f, -1.0f)},		//D
		{D3DXVECTOR3(+1.0f, -1.0f, -1.0f)},		//D
		{D3DXVECTOR3(+1.0f, -1.0f, +1.0f)},		//H
		{D3DXVECTOR3(-1.0f, -1.0f, +1.0f)},		//E

		{D3DXVECTOR3(+1.0f, -1.0f, +1.0f)},		//H
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f)},		//G
		{D3DXVECTOR3(-1.0f, +1.0f, +1.0f)},		//F
		{D3DXVECTOR3(-1.0f, +1.0f, +1.0f)},		//F
		{D3DXVECTOR3(-1.0f, -1.0f, +1.0f)},		//E
		{D3DXVECTOR3(+1.0f, -1.0f, +1.0f)},		//H

		{D3DXVECTOR3(-1.0f, +1.0f, -1.0f)},		//B
		{D3DXVECTOR3(-1.0f, +1.0f, +1.0f)},		//F
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f)},		//G
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f)},		//G
		{D3DXVECTOR3(+1.0f, +1.0f, -1.0f)},		//C
		{D3DXVECTOR3(-1.0f, +1.0f, -1.0f)},		//B*/


	/*Vertex vertices[] =
    {
		{D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(-1.0f, -1.0f, -1.0f), WHITE, WHITE},		//A - 0
		{D3DXVECTOR3(-1.0f, +1.0f, -1.0f), D3DXVECTOR3(-1.0f, +1.0f, -1.0f), WHITE, WHITE},		//B - 1
		{D3DXVECTOR3(+1.0f, +1.0f, -1.0f), D3DXVECTOR3(+1.0f, +1.0f, -1.0f), WHITE, WHITE},		//C - 2
		{D3DXVECTOR3(+1.0f, -1.0f, -1.0f), D3DXVECTOR3(+1.0f, -1.0f, -1.0f), WHITE, WHITE},		//D - 3

		{D3DXVECTOR3(-1.0f, -1.0f, +1.0f), D3DXVECTOR3(-1.0f, -1.0f, +1.0f), WHITE, WHITE},		//E - 4
		{D3DXVECTOR3(-1.0f, +1.0f, +1.0f), D3DXVECTOR3(-1.0f, +1.0f, +1.0f), WHITE, WHITE},		//F - 5
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f), D3DXVECTOR3(+1.0f, +1.0f, +1.0f), WHITE, WHITE},		//G - 6
		{D3DXVECTOR3(+1.0f, -1.0f, +1.0f), D3DXVECTOR3(+1.0f, -1.0f, +1.0f), WHITE, WHITE},		//H - 7
		
    };*/


	/*// Create indices
    unsigned int indices[] = {	0, 1, 2, 2, 3, 0,
								1, 5, 6, 6, 2, 1,
								4, 0, 3, 3, 7, 4,
								4, 5, 1, 1, 0, 4,
								3, 2, 6, 6, 7, 3,
								7, 6, 5, 5, 4, 7};*/