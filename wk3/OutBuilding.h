#ifndef OUTBUILD_H
#define OUTBUILD_H

#include "Vertex.h"
#include "Effects.h"
#include "Light.h"
#include <string>
#include <vector>

class OutBuilding{
public:
	struct InitInfo{
		std::wstring LayerMapFilename0;
		std::wstring LayerMapFilename1;
		std::wstring LayerMapFilename2;
		std::wstring BlendMapFilename;
		std::wstring SpecMapFilename;
	};
	OutBuilding();
	~OutBuilding();
	void init(ID3D10Device* device, const InitInfo& initInfo, float w, float h, float d);
	void build(float w, float h, float d);
	void draw(D3DXVECTOR3 mEyePos, Light mLights[], int mLightType);
	void setTrans();
	void Translate(float x, float y, float z);
	D3DXMATRIX GetWorld();

protected:
	DWORD mNumVertices;
	DWORD mNumFaces;
	DWORD mNumIndices;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;

	InitInfo mInfo;

	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectTechnique* mTech;

	ID3D10ShaderResourceView* mLayer0;
	ID3D10ShaderResourceView* mLayer1;
	ID3D10ShaderResourceView* mLayer2;
	ID3D10ShaderResourceView* mBlendMap;
	ID3D10ShaderResourceView* mSpecMap;

	ID3D10EffectShaderResourceVariable* mfxLayer0Var;
	ID3D10EffectShaderResourceVariable* mfxLayer1Var;
	ID3D10EffectShaderResourceVariable* mfxLayer2Var;
	ID3D10EffectShaderResourceVariable* mfxBlendMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;

	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectVariable* mfxLightVar;
	ID3D10EffectScalarVariable* mfxLightType;

	typedef std::vector<VertexPNT> VertexPNTList;
	typedef std::vector<DWORD> IndexList;

	D3DXMATRIX world;
	D3DXVECTOR3 pos, theta, scale;	
	void buildVB(VertexPNTList& vertices);
	void buildIB(IndexList& indices);
};

#endif // GFXOBJ_H