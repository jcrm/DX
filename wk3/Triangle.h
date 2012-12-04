#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "GfxObj.h"

class Triangle: public GfxObj{
public:
	Triangle();
	~Triangle();
	void draw(D3DXVECTOR3 mEyePos, Light mLights[], int mLightType);
	void init(ID3D10Device* device);
	void DrawVertices();
	float GetPosX();
	float GetPosY();
	float GetPosZ();

private:

};

#endif 