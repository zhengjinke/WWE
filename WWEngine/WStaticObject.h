#pragma once
/*
	静态对象，继承自obj。没有动画；
*/
#include "Common.h"
#include "WObject.h"
#include "Light.h"

class Material {
	D3DMATERIAL9 mtrl;
public:
	void SetMaterial(LPDIRECT3DDEVICE9 device) {
		::ZeroMemory(&mtrl, sizeof(mtrl));
		mtrl.Ambient = D3DXCOLOR(0.8f, 0.5f, 0.7f, 1.0f);
		mtrl.Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
		mtrl.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);
		mtrl.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		device->SetMaterial(&mtrl);
	}
	D3DMATERIAL9 GetMtrl() {
		return mtrl;
	}
};

class WStaticObject :public WObject{
	ID3DXMesh* Mesh;
	Material *mt;
	unsigned short m_Type = -1;
public :
	WStaticObject(){}
	WStaticObject(float x, float y, float z ) {
		transform.position.x = x;
		transform.position.y = y;
		transform.position.z = z;
	}
	void CreateBox(IDirect3DDevice9*	g_pDevice) {
		m_Type = 1;
		D3DXCreateBox(g_pDevice, 1.0f, 1.0f, 1.0f, &Mesh, NULL); 
		mt = new Material();
		mt->SetMaterial(g_pDevice);
	}
	virtual void Draw(IDirect3DDevice9* g_pDevice) {
		g_pDevice->SetTransform(D3DTS_WORLD, &transform.identity);
		g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		Light light;
		light.SetLight(g_pDevice, Light::DIRECTIONLIGHT);
		g_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		g_pDevice->SetMaterial(&(mt->GetMtrl()));
		Mesh->DrawSubset(0);
	}
	virtual void Update(float m_deltaTime, CallbackHandler &callbackHandler) {}
	void Release() { Mesh->Release(); }
};