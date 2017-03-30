#include "WCamera.h"

WCamera::WCamera()
{
	SetType(ObjectType::Camera);
}

WCamera::WCamera(float x, float y, float z)
{
	SetType(ObjectType::Camera);
	transform.SetPos(x, y, z);
	m_upDir.x = 0.0f;
	m_upDir.y = 1.0f;
	m_upDir.z = 0.0f;
}

void WCamera::SetTarget(float x, float y, float z)
{
	target.x = x;
	target.y = y;
	target.z = z;
}

void WCamera::SetCamera(IDirect3DDevice9*	g_pDevice, ID3DXEffect* g_pEffect)
{
	SetTarget(transform.identity._31 + transform.position.x, transform.identity._32 + transform.position.y, transform.identity._33 + transform.position.z);
	D3DXVECTOR3 vEye(transform.position.x, transform.position.y, transform.position.z);
	D3DXVECTOR3 vAt(target.x, target.y, target.z);
	D3DXVECTOR3 vUp(m_upDir.x, m_upDir.y, m_upDir.z);
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	g_pDevice->SetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixPerspectiveFovLH(&proj, FOV, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, VIEWDISTANCE);
	g_pDevice->SetTransform(D3DTS_PROJECTION, &proj);
}
