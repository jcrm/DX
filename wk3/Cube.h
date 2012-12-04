#ifndef CUBE_H
#define CUBE_H

#include "GfxObj.h"

class Cube: public GfxObj{
public:
	Cube();
	~Cube();
	void draw(D3DXVECTOR3 mEyePos, Light mLights[], int mLightType);
	void draw();
	void init(ID3D10Device* device, const InitInfo& initInfo);
	void init(ID3D10Device* device);
	void buildVB();
	void buildIB();
	float GetPosX();
	float GetPosY();
	float GetPosZ();

private:
};
#endif 