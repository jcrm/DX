#ifndef TERRAIN_H
#define TERRAIN_H

#include "D3DUtil.h"
#include "GfxObj.h"
#include <string>
#include <vector>

class Terrain : public GfxObj{
public:
	struct InitInfo{
		std::wstring HeightmapFilename;
		std::wstring LayerMapFilename0;
		std::wstring LayerMapFilename1;
		std::wstring LayerMapFilename2;
		std::wstring LayerMapFilename3;
		std::wstring LayerMapFilename4;
		std::wstring BlendMapFilename;
		float HeightScale;
		float HeightOffset;
		UINT NumRows;
		UINT NumCols;
		float CellSpacing;
	};

public:
	Terrain();
	~Terrain();

	float width()const;
	float depth()const;
	float getHeight(float x, float y)const;

	void init(ID3D10Device* device, const InitInfo& initInfo);

	void setDirectionToSun(const D3DXVECTOR3& v);

	void draw();

private:
	void loadHeightmap();
	void smooth();
	bool inBounds(UINT i, UINT j);
	float average(UINT i, UINT j);
	void buildVB();
	void buildIB();

private:
	InitInfo mInfo;

	std::vector<float> mHeightmap;

	ID3D10EffectVectorVariable* mfxDirToSunVar;
	
};

#endif // TERRAIN_H