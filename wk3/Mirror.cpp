#include "Mirror.h"
#include "Effects.h"
#include "TextureMgr.h"
#include "InputLayouts.h"
#include "Camera.h"
#include "Light.h"
#include "D3DUtil.h"

Mirror::Mirror():mVB(0){
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixIdentity(&mProj);
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mWVP);
	D3DXMatrixIdentity(&mIdentityTexMtx);
}
Mirror::~Mirror(){

}
void Mirror::init(ID3D10Device* device, const InitInfo& initInfo){
	md3dDevice = device;

	mTech			 =fx::MirrorFX->GetTechniqueByName("MirrorTech");
	mfxWorldVar		 =fx::MirrorFX->GetVariableByName("gWorld")->AsMatrix();
	mfxEyePosVar	 =fx::MirrorFX->GetVariableByName("gEyePosW");
	mfxLightVar		 =fx::MirrorFX->GetVariableByName("gLight");
	mfxWVPVar		 =fx::MirrorFX->GetVariableByName("gWVP")->AsMatrix();
	mfxDiffuseMapVar =fx::MirrorFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar	 =fx::MirrorFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar	 =fx::MirrorFX->GetVariableByName("gTexMtx")->AsMatrix();

	mNumVertices=	6;
	mNumFaces=		2;

	mInfo = initInfo;

	mMirrorDiffuseMapRV  = GetTextureMgr().createTex(initInfo.LayerMapFilename0);
	mSpecMap  = GetTextureMgr().createTex(initInfo.SpecMapFilename);

	CreateDesc();
	build();

}
void Mirror::CreateDesc(){
	D3D10_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable		= true;
	dsDesc.DepthWriteMask	= D3D10_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc		= D3D10_COMPARISON_LESS;
	dsDesc.StencilEnable	= true;
	dsDesc.StencilReadMask	= 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	dsDesc.FrontFace.StencilFailOp		= D3D10_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp	= D3D10_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp		= D3D10_STENCIL_OP_REPLACE;
	dsDesc.FrontFace.StencilFunc		= D3D10_COMPARISON_ALWAYS;

	dsDesc.BackFace.StencilFailOp		= D3D10_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp	= D3D10_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilPassOp		= D3D10_STENCIL_OP_REPLACE;
	dsDesc.BackFace.StencilFunc			= D3D10_COMPARISON_ALWAYS;

	HR(md3dDevice->CreateDepthStencilState(&dsDesc,&mDrawMirrorDSS));

	dsDesc.DepthEnable				= true;
	dsDesc.DepthWriteMask			= D3D10_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc				= D3D10_COMPARISON_ALWAYS;
	dsDesc.FrontFace.StencilPassOp	= D3D10_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc	= D3D10_COMPARISON_EQUAL;

	HR(md3dDevice->CreateDepthStencilState(&dsDesc,&mDrawReflectionDSS));
	
	D3D10_BLEND_DESC blendDesc = {0};
	blendDesc.AlphaToCoverageEnable		= false;
	blendDesc.BlendEnable[0]			= true;
	blendDesc.SrcBlend					= D3D10_BLEND_BLEND_FACTOR;
	blendDesc.DestBlend					= D3D10_BLEND_INV_BLEND_FACTOR;
	blendDesc.BlendOp					= D3D10_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha				= D3D10_BLEND_ONE;
	blendDesc.DestBlendAlpha			= D3D10_BLEND_ZERO;
	blendDesc.BlendOpAlpha				= D3D10_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask[0]	= D3D10_COLOR_WRITE_ENABLE_ALL;

	HR(md3dDevice->CreateBlendState(&blendDesc,&mDrawReflectionBS));
}
void Mirror::build(){
	VertexPNTList vertices;
	VertexPNT v;

	v.Update(0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f);
	vertices.push_back(v);

	v.Update(0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f);
	vertices.push_back(v);

	v.Update(5.0f,5.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f);
	vertices.push_back(v);

	v.Update(5.0f,5.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f);
	vertices.push_back(v);

	v.Update(5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
	vertices.push_back(v);

	v.Update(0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f);
	vertices.push_back(v);

	mNumVertices = (UINT)vertices.size();

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
void Mirror::draw(){
	md3dDevice->OMSetDepthStencilState(0,0);
	float blendFactors[]= {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0,blendFactors,0xFFFFFFFF);
	md3dDevice->IASetInputLayout(InputLayout::PosNormalTex);
	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);   

	mView = GetCamera().view();
	mProj = GetCamera().proj();
	
	Light mParallelLight;
	mParallelLight.dir		= D3DXVECTOR3(0.57735f, -0.57735f,0.57735f);
	mParallelLight.ambient	= D3DXCOLOR(0.4f,0.4f,0.4f,1.0f);
	mParallelLight.diffuse	= D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	mParallelLight.specular	= D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	mEyePos = GetCamera().position();

	D3D10_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);
	for(UINT p = 0; p< techDesc.Passes; ++p){
		
		ID3D10EffectPass* pass = mTech->GetPassByIndex(p);
		
		mfxEyePosVar->SetRawValue(&mEyePos, 0, sizeof(D3DXVECTOR3));
		mfxLightVar->SetRawValue(&mParallelLight,0,sizeof(Light));

		UINT stride = sizeof(VertexPNT);
		UINT offset = 0;
		/*		Draw Mirror		*/
		mWVP = mWorld*mView*mProj;
		mfxWVPVar->SetMatrix((float*)&mWVP);
		mfxWorldVar->SetMatrix((float*)&mWorld);
		mfxSpecMapVar->SetResource(mSpecMap);
		mfxTexMtxVar->SetMatrix((float*)&mIdentityTexMtx);
		md3dDevice->IASetVertexBuffers(0,1,&mVB,&stride,&offset);

		md3dDevice->OMSetDepthStencilState(mDrawMirrorDSS,1);

		mfxDiffuseMapVar->SetResource(mMirrorDiffuseMapRV);
		pass->Apply(0);
		md3dDevice->Draw(mNumVertices,0);

		md3dDevice->OMSetDepthStencilState(0,0);
	}
}