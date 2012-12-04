#ifndef INPUTLAYOUTS_H
#define INPUTLAYOUTS_H

#include "D3DUtil.h"

namespace InputLayout
{
	extern ID3D10InputLayout* Pos;
	extern ID3D10InputLayout* PosNormalTex;
	extern ID3D10InputLayout* PosTangentNormalTex;

	void InitAll(ID3D10Device* device);
	void DestroyAll();
};

#endif // INPUTLAYOUTS_H