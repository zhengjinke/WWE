#include "WText.h"
WText::WText(IDirect3DDevice9*	g_pDevice) {
	D3DXCreateFont(g_pDevice, 50, 0, FW_BOLD, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, "Arial", &g_pFont_Arial_50_BOLD);

	D3DXCreateFont(g_pDevice, 18, 0, FW_BOLD, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, "Arial", &g_pFont_Arial_20_BOLD);

	D3DXCreateFont(g_pDevice, 50, 0, FW_THIN, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, "Arial", &g_pFont_Arial_50_THIN);

	D3DXCreateFont(g_pDevice, 18, 0, FW_THIN, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, "Arial", &g_pFont_Arial_20_THIN);
}

void WText::OnLostDevice()
{
	g_pFont_Arial_50_BOLD->OnLostDevice();
	g_pFont_Arial_20_BOLD->OnLostDevice();
	g_pFont_Arial_50_THIN->OnLostDevice();
	g_pFont_Arial_20_THIN->OnLostDevice();
}

void WText::OnResetDevice()
{
	g_pFont_Arial_50_BOLD->OnResetDevice();
	g_pFont_Arial_20_BOLD->OnResetDevice();
	g_pFont_Arial_50_THIN->OnResetDevice();
	g_pFont_Arial_20_THIN->OnResetDevice();
}
void WText::Write(int tlx, int tly, int brx, int bry, char *szStr, DWORD nType) {
	ID3DXFont *font = NULL;
	if (nType == 1) font = this->g_pFont_Arial_20_THIN;
	if (nType == 2) font = this->g_pFont_Arial_20_BOLD;
	if (nType == 3) font = this->g_pFont_Arial_50_THIN;
	if (nType == 3) font = this->g_pFont_Arial_50_BOLD;
	if (font == NULL) {
		messagebox("WriteText error");
		return;
	}
	RECT rc = { tlx, tly, brx, bry };
	font->DrawText(NULL, szStr, -1, &rc,  DT_NOCLIP, 0xFF000000);//| DT_CENTER |DT_VCENTER
}