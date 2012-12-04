#ifndef EFFECTS_H
#define EFFECTS_H

#include "D3DUtil.h"

namespace fx
{
	extern ID3D10Effect* SkyFX;
	extern ID3D10Effect* TerrainFX;
	extern ID3D10Effect* MeshFX;
	extern ID3D10Effect* BuildShadowMapFX;
	extern ID3D10Effect* SphereFX;
	extern ID3D10Effect* CubeFX;
	extern ID3D10Effect* MirrorFX;
	extern ID3D10Effect* OutBuildingFX;
	extern ID3D10Effect* InBuildingFX;
	void InitAll(ID3D10Device* device);
	void DestroyAll();
};

#endif // EFFECTS_H