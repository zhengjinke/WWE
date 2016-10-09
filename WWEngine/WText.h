#pragma once
#include "Common.h"
class WText {
private:
	ID3DXFont*			g_pFont_Arial_50_BOLD = NULL;
	ID3DXFont*			g_pFont_Arial_20_BOLD = NULL;
	ID3DXFont*			g_pFont_Arial_50_THIN = NULL;
	ID3DXFont*			g_pFont_Arial_20_THIN = NULL;
public:
	WText(IDirect3DDevice9*	g_pDevice);
	void Write(int tlx, int tly, int brx, int bry, char *szStr, DWORD nType);
	void OnLostDevice();
	void OnResetDevice();
};