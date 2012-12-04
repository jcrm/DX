#ifndef BUILD_H
#define BUILD_H

#include "Vertex.h"
#include "Effects.h"
#include "Light.h"
#include "InBuilding.h"
#include "OutBuilding.h"
#include <string>
#include <vector>

class Building{
public:
	Building();
	~Building();
	void init(ID3D10Device* device, float d, float w, float h);
	void draw(D3DXVECTOR3 mEyePos, Light mLights[], int mLightType);
	void Translate(float x, float y, float z);

protected:
	InBuilding mIn;
	OutBuilding mOut;
};

#endif // GFXOBJ_H