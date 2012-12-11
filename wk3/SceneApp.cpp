//=============================================================================
// Triangle App.cpp based on Frank Lunas Colored Cube App
//
// Demonstrates coloring.
//
// Controls:
//		'A'/'D'/'W'/'S' - Rotate 
//
//=============================================================================

#include "D3DApp.h"
#include "Vertex.h"

class SceneApp : public D3DApp{
public:
	SceneApp(HINSTANCE hInstance);
	~SceneApp();

	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 

private:
	D3DXMATRIX mWVP;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	SceneApp theApp(hInstance);
	
	theApp.initApp();

	return theApp.run();
}

SceneApp::SceneApp(HINSTANCE hInstance)
: D3DApp(hInstance)
{
}

SceneApp::~SceneApp(){
	if( md3dDevice )
		md3dDevice->ClearState();
}

void SceneApp::initApp(){
	D3DApp::initApp();

	fx::InitAll(md3dDevice);
	InputLayout::InitAll(md3dDevice);
	GetTextureMgr().init(md3dDevice);
	
	ID3D10ShaderResourceView* mEnvMapRV = GetTextureMgr().createCubeTex(L"grassenvmap1024.dds");
	mSky.init(md3dDevice, mEnvMapRV, 5000.0f);

	Terrain::InitInfo tii;
	tii.HeightmapFilename = L"mountainScene513.raw";
	tii.LayerMapFilename0 = L"grass.dds";
	tii.LayerMapFilename1 = L"lightdirt.dds";
	tii.LayerMapFilename2 = L"darkdirt.dds";
	tii.LayerMapFilename3 = L"stone.dds";
	tii.LayerMapFilename4 = L"snow.dds";
	tii.BlendMapFilename  = L"blend.dds";
	tii.HeightScale  = 0.85f;
	tii.HeightOffset = -2.0f;
	tii.NumRows      = 513;
	tii.NumCols      = 513;
	tii.CellSpacing  = 1.0f;
	mLand.init(md3dDevice, tii);

	Cube::InitInfo cii;
	cii.LayerMapFilename0 = L"grass0.dds";
	cii.LayerMapFilename1 = L"stone2.dds";
	cii.LayerMapFilename2 = L"ground0.dds";
	cii.BlendMapFilename  = L"blendmap.dds";
	cii.SpecMapFilename   = L"defaultspec.dds";
	thisCube.init(md3dDevice, cii);
	thisSphere.init(md3dDevice,(3.142/5),10,10,cii);

	Mirror::InitInfo mii;
	mii.LayerMapFilename0	= L"ice.dds";
	mii.LayerMapFilename1	= L"WoodCrate02.dds";
	mii.SpecMapFilename		= L"defaultspec.dds";
	mMirror.init(md3dDevice,mii);

	mBuilding1.init(md3dDevice, 16, 24, 12);

	mLightType = 0;
	// Parallel light.
	mLights[0].dir      = D3DXVECTOR3(0.57735f, -0.57735f, 0.57735f);
	mLights[0].ambient  = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	mLights[0].diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mLights[0].specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mLand.setDirectionToSun(-mLights[0].dir);

	//Point light.
	mLights[1].dir      = D3DXVECTOR3(0.57735f, -0.57735f, 0.57735f);
	mLights[1].ambient  = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	mLights[1].diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mLights[1].specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mLights[1].att.x = 0.0f;
	mLights[1].att.y = 0.1f;
	mLights[1].att.z = 0.0f;
	mLights[1].range = 50.0f;

	//Spot light.
	mLights[2].dir      = D3DXVECTOR3(0.57735f, -0.57735f, 0.57735f);
	mLights[2].ambient  = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	mLights[2].diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mLights[2].specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mLights[2].att.x = 1.0f;
	mLights[2].att.y = 0.0f;
	mLights[2].att.z = 0.0f;
	mLights[2].spotPow = 64.0f;
	mLights[2].range = 50.0f;
}

void SceneApp::onResize(){
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	//create projection matrix from field of view
	GetCamera().setLens(0.25f*PI, aspect, 1.0f, 1000.0f);
}

void SceneApp::updateScene(float dt){
	D3DApp::updateScene(dt);
	GetCamera().Update();
	GetCamera().Look();
}

void SceneApp::drawScene(){
	D3DApp::drawScene();
	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
	
	mLand.draw();
	thisCube.draw(mEyePos, mLights, mLightType);
	thisSphere.Translate(5.0f,0.0f,0.0f);
	thisSphere.draw(mEyePos, mLights, mLightType);
	mBuilding1.Translate(0.0f,-2.0f,5.0f);
	mBuilding1.draw(mEyePos, mLights, mLightType);
	mMirror.Translate(8.0f,-2.0f,20.8f);
	mMirror.draw(mEyePos, mLights);
	// Draw sky last to save fill rate.
	mSky.draw();
	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {5, 5, 0, 0};
	md3dDevice->RSSetState(0);
	mFont->DrawText(0, mFrameStats.c_str(), -1, &R, DT_NOCLIP, BLACK);
	mSwapChain->Present(0, 0);
}