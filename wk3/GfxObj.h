#ifndef GFXOBJ_H
#define GFXOBJ_H

/*** COMPILER OPTIONS ***/

//#ifdef _MSVC
#pragma warning(disable:4244)	// double -> float conversion
#define _CRT_SECURE_NO_WARNINGS
//#endif //_MSVC
#include "D3DUtil.h"
#include "GameTimer.h"
#include "Vertex.h"
#include "Light.h"
#include <string>
#include <vector>

#pragma warning(disable:4305) // double -> float


class GfxObj{
public:
	virtual struct InitInfo{
		std::wstring LayerMapFilename0;
		std::wstring LayerMapFilename1;
		std::wstring LayerMapFilename2;
		std::wstring BlendMapFilename;
		std::wstring SpecMapFilename;
	};
	GfxObj();
	~GfxObj();
	virtual void init(ID3D10Device* device);
	virtual void draw(D3DXVECTOR3 mEyePos, Light mLights[], int mLightType);
	virtual void draw();
	virtual void drawR();
	void setTrans();
	void Scale(float x, float y, float z);
	void Translate(float x, float y, float z);
	D3DXMATRIX GetWorld();

protected:
	DWORD mNumVertices;
	DWORD mNumFaces;
	DWORD mNumIndices;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	bool indexed;
	bool reflected;
	int mVertexType;

	InitInfo mInfo;

	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectTechnique* mTech;

	ID3D10ShaderResourceView* mLayer0;
	ID3D10ShaderResourceView* mLayer1;
	ID3D10ShaderResourceView* mLayer2;
	ID3D10ShaderResourceView* mLayer3;
	ID3D10ShaderResourceView* mLayer4;
	ID3D10ShaderResourceView* mBlendMap;
	ID3D10ShaderResourceView* mSpecMap;

	ID3D10EffectShaderResourceVariable* mfxLayer0Var;
	ID3D10EffectShaderResourceVariable* mfxLayer1Var;
	ID3D10EffectShaderResourceVariable* mfxLayer2Var;
	ID3D10EffectShaderResourceVariable* mfxLayer3Var;
	ID3D10EffectShaderResourceVariable* mfxLayer4Var;
	ID3D10EffectShaderResourceVariable* mfxBlendMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;

	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectVariable* mfxLightVar;
	ID3D10EffectScalarVariable* mfxLightType;

	typedef std::vector<VertexPNT> VertexPNTList;
	typedef std::vector<Vertex> VertexList;
	typedef std::vector<DWORD> IndexList;

	D3DXMATRIX world;
	D3DXVECTOR3 pos, theta, scale;	
	virtual void buildVB(VertexPNTList& vertices);
	virtual void buildVB(VertexList& vertices);
	virtual void buildIB(IndexList& indices);
};

#endif // GFXOBJ_H