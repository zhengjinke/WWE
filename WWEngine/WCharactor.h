#pragma once
#include "WObject.h"
#include "skinnedMesh.h"
#include "Common.h"
class WCharactor : public WObject{
public:
	SkinnedMesh m_mesh;
	ID3DXAnimationController *m_aniController;
	vector<string> animations;
	WCharactor();
	WCharactor(char * szFile, char * szName, float x, float y, float z);
	void ShowAnimations();
	virtual void Draw(IDirect3DDevice9*	g_pDevice);
};