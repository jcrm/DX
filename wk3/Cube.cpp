#include "Effects.h"
#include "TextureMgr.h"
#include "InputLayouts.h"
#include "Cube.h"

Cube::Cube(){

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
void Cube::draw(){
	GfxObj::draw();
}
float Cube::GetPosX(){
	return pos.x;
}
float Cube::GetPosY(){
	return pos.y;
}
float Cube::GetPosZ(){
	return pos.z;
}
Cube::~Cube(){

}
void Cube::buildVB(){
	VertexPNTList vertices;
	VertexPNT v;
	//Front
	v.normal = D3DXVECTOR3( 0.0f,  0.0f, -1.0f);
	v.pos = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	v.texC = D3DXVECTOR2(0.0,0.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(-1.0f, +1.0f, -1.0f);
	v.texC = D3DXVECTOR2(0.0,1.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(+1.0f, +1.0f, -1.0f);
	v.texC = D3DXVECTOR2(1.0,1.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(+1.0f, -1.0f, -1.0f);
	v.texC = D3DXVECTOR2(1.0,0.0);
	vertices.push_back(v);

	//Left Face
	v.normal = D3DXVECTOR3( -1.0f,  0.0f, 0.0f);
	v.pos = D3DXVECTOR3(-1.0f, -1.0f, +1.0f);
	v.texC = D3DXVECTOR2(0.0,0.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(-1.0f, +1.0f, +1.0f);
	v.texC = D3DXVECTOR2(0.0,1.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(-1.0f, +1.0f, -1.0f);
	v.texC = D3DXVECTOR2(1.0,1.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	v.texC = D3DXVECTOR2(1.0,0.0);
	vertices.push_back(v);

	//Top
	v.normal = D3DXVECTOR3( 0.0f,  1.0f, 0.0f);
	v.pos = D3DXVECTOR3(-1.0f, +1.0f, -1.0f);
	v.texC = D3DXVECTOR2(0.0,0.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(-1.0f, +1.0f, +1.0f);
	v.texC = D3DXVECTOR2(0.0,1.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(+1.0f, +1.0f, +1.0f);
	v.texC = D3DXVECTOR2(1.0,1.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(+1.0f, +1.0f, -1.0f);
	v.texC = D3DXVECTOR2(1.0,0.0);
	vertices.push_back(v);

	//Right
	v.normal = D3DXVECTOR3( 1.0f,  0.0f, 0.0f);
	v.pos = D3DXVECTOR3(+1.0f, -1.0f, -1.0f);
	v.texC = D3DXVECTOR2(0.0,0.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(+1.0f, +1.0f, -1.0f);
	v.texC = D3DXVECTOR2(0.0,1.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(+1.0f, +1.0f, +1.0f);
	v.texC = D3DXVECTOR2(1.0,1.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(+1.0f, -1.0f, +1.0f);
	v.texC = D3DXVECTOR2(1.0,0.0);
	vertices.push_back(v);

	//Bottom
	v.normal = D3DXVECTOR3( 0.0f,  -1.0f, 0.0f);
	v.pos = D3DXVECTOR3(-1.0f, -1.0f, +1.0f);
	v.texC = D3DXVECTOR2(0.0,0.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	v.texC = D3DXVECTOR2(0.0,1.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(+1.0f, -1.0f, -1.0f);
	v.texC = D3DXVECTOR2(1.0,1.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(+1.0f, -1.0f, +1.0f);
	v.texC = D3DXVECTOR2(1.0,0.0);
	vertices.push_back(v);

	//Back
	v.normal = D3DXVECTOR3( 0.0f,  0.0f, 1.0f);
	v.pos = D3DXVECTOR3(+1.0f, -1.0f, +1.0f);
	v.texC = D3DXVECTOR2(0.0,0.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(+1.0f, +1.0f, +1.0f);
	v.texC = D3DXVECTOR2(0.0,1.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(-1.0f, +1.0f, +1.0f);
	v.texC = D3DXVECTOR2(1.0,1.0);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(-1.0f, -1.0f, +1.0f);
	v.texC = D3DXVECTOR2(1.0,0.0);
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
void Cube::init(ID3D10Device* device){
	md3dDevice = device;
	mVertexType= 2;
	mTech			= fx::CubeFX->GetTechniqueByName("MultiTexTech");
	mfxWVPVar		= fx::CubeFX->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVar		= fx::CubeFX->GetVariableByName("gWorld")->AsMatrix();
	mfxLayer0Var	= fx::CubeFX->GetVariableByName("gLayer0")->AsShaderResource();
	mfxLayer1Var	= fx::CubeFX->GetVariableByName("gLayer1")->AsShaderResource();
	mfxLayer2Var	= fx::CubeFX->GetVariableByName("gLayer2")->AsShaderResource();
	mfxBlendMapVar	= fx::CubeFX->GetVariableByName("gBlendMap")->AsShaderResource();
	mfxSpecMapVar	= fx::CubeFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxEyePosVar	= fx::CubeFX->GetVariableByName("gEyePosW");
	mfxLightVar		= fx::CubeFX->GetVariableByName("gLight");
	mfxLightType	= fx::CubeFX->GetVariableByName("gLightType")->AsScalar();

	mNumVertices=	8;
	mNumFaces=		12;
	mNumIndices =	36;

	buildVB();
	buildIB();
}
void Cube::init(ID3D10Device* device, const InitInfo& initInfo){
	md3dDevice = device;
	mVertexType= 2;
	mTech			= fx::CubeFX->GetTechniqueByName("MultiTexTech");
	mfxWVPVar		= fx::CubeFX->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVar		= fx::CubeFX->GetVariableByName("gWorld")->AsMatrix();
	mfxLayer0Var	= fx::CubeFX->GetVariableByName("gLayer0")->AsShaderResource();
	mfxLayer1Var	= fx::CubeFX->GetVariableByName("gLayer1")->AsShaderResource();
	mfxLayer2Var	= fx::CubeFX->GetVariableByName("gLayer2")->AsShaderResource();
	mfxBlendMapVar	= fx::CubeFX->GetVariableByName("gBlendMap")->AsShaderResource();
	mfxSpecMapVar	= fx::CubeFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxEyePosVar	= fx::CubeFX->GetVariableByName("gEyePosW");
	mfxLightVar		= fx::CubeFX->GetVariableByName("gLight");
	mfxLightType	= fx::CubeFX->GetVariableByName("gLightType")->AsScalar();
		

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