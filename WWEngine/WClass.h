#pragma once
#include <d3dx9.h>
#define WMATRIX D3DXMATRIX
#define VECTOR3 D3DVECTOR
extern   float	m_show;
class CallbackHandler : public ID3DXAnimationCallbackHandler
{
public:
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData)
	{
		m_show = 0.25f;
		return D3D_OK;
	}
};