#include "Mirror.h"
#include "Effects.h"
#include "TextureMgr.h"
#include "InputLayouts.h"
#include "Camera.h"
#include "Light.h"
#include "D3DUtil.h"

Mirror::Mirror():scale(1,1,1),pos(0,0,0),theta(0,0,0){
	D3DXMatrixTranslation(&mReflectWorld, 2.0f, 1.0f, -4.0f);
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
	mCrate.initR(md3dDevice);
	mTech			 =fx::MirrorFX->GetTechniqueByName("MirrorTech");
	mfxWVPVar		 =fx::MirrorFX->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVar		 =fx::MirrorFX->GetVariableByName("gWorld")->AsMatrix();
	mfxDiffuseMapVar =fx::MirrorFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar	 =fx::MirrorFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxEyePosVar	 =fx::MirrorFX->GetVariableByName("gEyePosW");
	mfxLightVar		 =fx::MirrorFX->GetVariableByName("gLight");
	mfxTexMtxVar	 =fx::MirrorFX->GetVariableByName("gTexMtx")->AsMatrix();


	mInfo = initInfo;

	mNumVertices=	6;
	mNumFaces=		2;

	CreateDesc();
	build();

	mMirrorDiffuseMapRV		= GetTextureMgr().createTex(initInfo.LayerMapFilename0);
	mObjectsDiffuseMapRV	= GetTextureMgr().createTex(initInfo.LayerMapFilename1);
	mSpecMap				= GetTextureMgr().createTex(initInfo.SpecMapFilename);

}
void Mirror::CreateDesc(){

	D3D10_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D10_RASTERIZER_DESC));
	rsDesc.FillMode = D3D10_FILL_SOLID;
	rsDesc.CullMode = D3D10_CULL_BACK;
	rsDesc.FrontCounterClockwise = true;

	HR(md3dDevice->CreateRasterizerState(&rsDesc, &mCullCWRS));

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
void Mirror::draw(D3DXVECTOR3 mEyePos, Light mLights[]){
	setTrans();
	md3dDevice->OMSetDepthStencilState(0,0);
	float blendFactors[]= {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0,blendFactors,0xFFFFFFFF);
	md3dDevice->IASetInputLayout(InputLayout::PosNormalTex);
	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);   

	D3DXMATRIX mView = GetCamera().view();
	D3DXMATRIX mProj = GetCamera().proj();
	

	D3D10_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);
	for(UINT p = 0; p< techDesc.Passes; ++p){
		
		ID3D10EffectPass* pass = mTech->GetPassByIndex(p);
		
		mfxEyePosVar->SetRawValue(&mEyePos, 0, sizeof(D3DXVECTOR3));
		mfxLightVar->SetRawValue(&mLights[0],0,sizeof(Light));

		UINT stride = sizeof(VertexPNT);
		UINT offset = 0;

		mWVP = mReflectWorld*mWorld*mView*mProj;
		mfxWVPVar->SetMatrix((float*)&mWVP);
		mfxWorldVar->SetMatrix((float*)&mReflectWorld);
		mfxDiffuseMapVar->SetResource(mObjectsDiffuseMapRV);
		mfxSpecMapVar->SetResource(mSpecMap);
		mfxTexMtxVar->SetMatrix((float*)&mIdentityTexMtx);
        pass->Apply(0);
		mCrate.draw();

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
		//draw reflected world
		D3DXPLANE mirrorPlane(0.0, 0.0f, 1, 0.0f); // xy plane
		 
		D3DXMATRIX R;
		_tmp_D3DXMatrixReflect(&R, &mirrorPlane);
		R*=mWorld;
		D3DXMATRIX W = mReflectWorld*R;
		mWVP = W*mView*mProj;
		mfxWVPVar->SetMatrix((float*)&mWVP);
		mfxWorldVar->SetMatrix((float*)&W);
		mfxDiffuseMapVar->SetResource(mObjectsDiffuseMapRV);
		mfxSpecMapVar->SetResource(mSpecMap);
		mfxTexMtxVar->SetMatrix((float*)&mIdentityTexMtx);

		D3DXVECTOR3 oldDir = mLights[0].dir;
		D3DXVec3TransformNormal(&mLights[0].dir, &mLights[0].dir, &R);
		mfxLightVar->SetRawValue(&mLights[0], 0, sizeof(Light));	
		pass->Apply(0);

		md3dDevice->RSSetState(mCullCWRS);
		float blendf[] = {0.65f, 0.65f, 0.65f, 1.0f};
		md3dDevice->OMSetBlendState(mDrawReflectionBS, blendf, 0xffffffff); 
		md3dDevice->OMSetDepthStencilState(mDrawReflectionDSS, 1);
		mCrate.draw();

		md3dDevice->OMSetDepthStencilState(0, 0);
		md3dDevice->OMSetBlendState(0, blendf, 0xffffffff);
		md3dDevice->RSSetState(0);	
		mLights[0].dir = oldDir; // restore
	}
}
void Mirror::setTrans(void){
	D3DXMATRIX m;
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixScaling(&m, scale.x, scale.y, scale.z);
	mWorld*=m;
	D3DXMatrixRotationY(&m, theta.x);
	mWorld*=m;
	D3DXMatrixRotationY(&m, theta.y);
	mWorld*=m;
	D3DXMatrixRotationY(&m, theta.z);
	mWorld*=m;
	D3DXMatrixTranslation(&m, pos.x, pos.y, pos.z);
	mWorld*=m;
}
void Mirror::Translate(float x, float y, float z){
	pos.x = x;
	pos.y = y;
	pos.z = z;
	setTrans();
}
D3DXMATRIX* Mirror::_tmp_D3DXMatrixReflect(D3DXMATRIX* pMat, const D3DXPLANE* pPlane)
    {
       if(!pMat || !pPlane) return NULL;

       D3DXPLANE n;
       D3DXPlaneNormalize(&n, pPlane);

       pMat->_11 = -2.0f * n.a * n.a + 1.0f;
       pMat->_12 = -2.0f * n.b * n.a;
       pMat->_13 = -2.0f * n.c * n.a;
       pMat->_14 = 0.0f;

       pMat->_21 = -2.0f * n.a * n.b;
       pMat->_22 = -2.0f * n.b * n.b + 1.0f;
       pMat->_23 = -2.0f * n.c * n.b;
       pMat->_24 = 0.0f;

       pMat->_31 = -2.0f * n.a * n.c;
       pMat->_32 = -2.0f * n.b * n.c;
       pMat->_33 = -2.0f * n.c * n.c + 1.0f;
       pMat->_34 = 0.0f;

       pMat->_41 = -2.0f * n.a * n.d;
       pMat->_42 = -2.0f * n.b * n.d;
       pMat->_43 = -2.0f * n.c * n.d;
       pMat->_44 = 1.0f;

       return pMat;
    }
