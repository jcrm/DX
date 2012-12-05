#include "Mirror.h"
#include "Effects.h"
#include "D3DUtil.h"

Mirror::Mirror(){

}
Mirror::~Mirror(){

}
void Mirror::init(ID3D10Device* device, const InitInfo& initInfo){
	md3dDevice = device;

	mTech			 =fx::MirrorFX->GetTechniqueByName("MirrorTech");
	mfxWVPVar		 =fx::MirrorFX->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVar		 =fx::MirrorFX->GetVariableByName("gWVP")->AsMatrix();
	mfxEyePosVar	 =fx::MirrorFX->GetVariableByName("gWVP");
	mfxLightVar		 =fx::MirrorFX->GetVariableByName("gWVP");
	mfxDiffuseMapVar =fx::MirrorFX->GetVariableByName("gWVP")->AsShaderResource();
	mfxSpecMapVar	 =fx::MirrorFX->GetVariableByName("gWVP")->AsShaderResource();
	mfxTexMtxVar	 =fx::MirrorFX->GetVariableByName("gWVP")->AsMatrix();

	mNumVertices=	6;
	mNumFaces=		2;
}