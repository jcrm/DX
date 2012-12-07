#ifndef MIRROR_H
#define MIRROR_H

#include <string>
#include "Effects.h"
#include "D3DUtil.h"
#include "Vertex.h"

class Mirror{
public:
	struct InitInfo{
		std::wstring LayerMapFilename0;
		std::wstring LayerMapFilename1;
		std::wstring LayerMapFilename2;
		std::wstring LayerMapFilename3;
		std::wstring SpecMapFilename;
	};
	Mirror();
	~Mirror();
	void init(ID3D10Device* device, const InitInfo& initInfo);
	void build();
	void draw();

protected:
	DWORD mNumVertices;
	DWORD mNumFaces;
	DWORD mNumIndices;

	InitInfo mInfo;
	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;

	ID3D10BlendState* mDrawReflectionBS;
	ID3D10DepthStencilState* mDrawMirrorDSS;
	ID3D10DepthStencilState* mDrawReflectionDSS;

	ID3D10EffectTechnique* mTech;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectVariable* mfxLightVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;

	ID3D10ShaderResourceView* mMirrorDiffuseMapRV;
	ID3D10ShaderResourceView* mSpecMap;

	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;

	D3DXMATRIX mReflectWorld;
	D3DXMATRIX mWorld;

	D3DXMATRIX mIdentityTexMtx;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mWVP;

	D3DXVECTOR3 mEyePos;

	typedef std::vector<VertexPNT> VertexPNTList;

	void CreateDesc();
};

#endif