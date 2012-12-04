#ifndef SKYDOME_H
#define SKYDOME_H

#include "D3DUtil.h"
#include "GfxObj.h"

class Sky : public GfxObj
{
public:
	Sky();
	~Sky();

	void init(ID3D10Device* device, ID3D10ShaderResourceView* cubemap, float radius);

	void draw();

private:
	Sky(const Sky& rhs);
	Sky& operator=(const Sky& rhs);

private:

	ID3D10ShaderResourceView* mCubeMap;
	ID3D10EffectShaderResourceVariable* mfxCubeMapVar;
};

#endif // SKY_H