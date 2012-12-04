#ifndef SPHERE_H
#define SPHERE_H

#include "D3DUtil.h"
#include "Vertex.h"
#include "GfxObj.h"


class Sphere: public GfxObj
{
public:
	Sphere();
	~Sphere();
	void buildVB();
	void buildIB();
	void init(ID3D10Device* device, float radius, UINT numSlices, UINT numStacks, const InitInfo& initInfo);
	void draw(D3DXVECTOR3 mEyePos, Light mLights[3], int mLightType);
	void DrawVertices();

private:
	float mRadius;
	UINT  mNumSlices;
	UINT  mNumStacks;
};

#endif // SPHERE_H
