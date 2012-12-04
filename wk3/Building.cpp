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
	obii.LayerMapFilename0 = L"grass0.dds";
	obii.LayerMapFilename1 = L"stone2.dds";
	obii.LayerMapFilename2 = L"ground0.dds";
	obii.BlendMapFilename  = L"blendmap.dds";
	obii.SpecMapFilename   = L"defaultspec.dds";
	mOut.init(device, obii, w, h, d);

	InBuilding::InitInfo ibii;
	ibii.LayerMapFilename0 = L"grass0.dds";
	ibii.LayerMapFilename1 = L"stone2.dds";
	ibii.LayerMapFilename2 = L"ground0.dds";
	ibii.BlendMapFilename  = L"blendmap.dds";
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