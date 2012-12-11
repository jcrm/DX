#include "Building.h"
#include "Camera.h"
#include "InputLayouts.h"
#include "TextureMgr.h"

Building::Building(){
}

Building::~Building(){
}
void Building::init (ID3D10Device* device, float d, float w, float h){
	
	OutBuilding::InitInfo obii;
	obii.LayerMapFilename0 = L"brick.dds";
	obii.LayerMapFilename1 = L"ceiling.dds";
	obii.LayerMapFilename2 = L"floor.dds";
	obii.SpecMapFilename   = L"defaultspec.dds";
	mOut.init(device, obii, w, h, d);

	InBuilding::InitInfo ibii;
	ibii.LayerMapFilename0 = L"wall.dds";
	ibii.LayerMapFilename1 = L"ceiling.dds";
	ibii.LayerMapFilename2 = L"floor.dds";
	ibii.SpecMapFilename   = L"defaultspec.dds";
	mIn.init(device, ibii, w, h, d);
}
void Building::draw(D3DXVECTOR3 mEyePos, Light mLights[], int mLightType){
	mIn.draw(mEyePos, mLights, mLightType);
	mOut.draw(mEyePos, mLights, mLightType);
}
void Building::Translate(float x, float y, float z){
	mIn.Translate(x, y, z);
	mOut.Translate(x, y, z);
}