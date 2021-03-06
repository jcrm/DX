//=======================================================================================
// d3dApp.h by Frank Luna (C) 2008 All Rights Reserved.
//
// Simple Direct3D demo application class.  
// Make sure you link: d3d10.lib d3dx10d.lib dxerr.lib dxguid.lib.
// Link d3dx10.lib for release mode builds instead of d3dx10d.lib.
//=======================================================================================


#ifndef D3DAPP_H
#define D3DAPP_H

#include "GfxObj.h"
#include "Triangle.h"
#include "Cube.h"
#include "D3DUtil.h"
#include "GameTimer.h"
#include "Light.h"
#include "Camera.h"
#include "Sphere.h"
#include "Effects.h"
#include "InputLayouts.h"
#include "TextureMgr.h"
#include "Sky.h"
#include "Mouse.h"
#include "DrawableTex2D.h"
#include "Terrain.h"
#include "Building.h"
#include <string>

class D3DApp{
public:
	D3DApp(HINSTANCE hInstance);
	virtual ~D3DApp();

	HINSTANCE getAppInst();
	HWND      getMainWnd();

	int run();

	Mouse MousePos;

	// Framework methods.  Derived client class overrides these methods to 
	// implement specific application requirements.

	virtual void initApp();
	virtual void onResize();// reset projection/etc
	virtual void updateScene(float dt);
	virtual void drawScene(); 
	virtual LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	void initMainWindow();
	void initDirect3D();
	
protected:

	HINSTANCE mhAppInst;
	HWND      mhMainWnd;
	bool      mAppPaused;
	bool      mMinimized;
	bool      mMaximized;
	bool      mResizing;

	GameTimer	mTimer;
	Cube		thisCube;
	Sphere		thisSphere;
	Terrain		mLand;
	Sky			mSky;
	Building	mBuilding1;

	Light mLights[3];
	int mLightType; // 0 (parallel), 1 (point), 2 (spot)
	D3DXVECTOR3 mEyePos;

	std::wstring mFrameStats;
 
	ID3D10Device*			md3dDevice;
	IDXGISwapChain*			mSwapChain;
	ID3D10Texture2D*		mDepthStencilBuffer;
	ID3D10RenderTargetView* mRenderTargetView;
	ID3D10DepthStencilView* mDepthStencilView;
	ID3DX10Font*			mFont;

	// Derived class should set these in derived constructor to customize starting values.
	std::wstring mMainWndCaption;
	D3D10_DRIVER_TYPE md3dDriverType;
	D3DXCOLOR mClearColor;
	int mClientWidth;
	int mClientHeight;
};




#endif // D3DAPP_H