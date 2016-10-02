#pragma once
#include "WObject.h"
#include "skinnedMesh.h"
#include "Common.h"
class WCharactor : public WObject{
public:
	WCharactor();
	WCharactor(char * szFile, char * szName, float x, float y, float z);
	SkinnedMesh m_mesh;
	virtual void Draw(IDirect3DDevice9*	g_pDevice);
};