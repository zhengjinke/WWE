#pragma once
#include "WObject.h"
#include "Common.h"
#define VIEWDISTANCE 500
#define FOV	 (D3DX_PI / 4.0f)
class WCamera :public WObject {
private:
	
	VECTOR3 target;
	VECTOR3 m_upDir;
	void SetTarget(float x, float y, float z);
public:
	WMATRIX proj;
	WMATRIX matView;
	WCamera();
	WCamera(float x,float y,float z);
	void SetCamera(IDirect3DDevice9* g_pDevice, ID3DXEffect* g_pEffect);
	virtual void Draw(IDirect3DDevice9*	g_pDevice){}
};
