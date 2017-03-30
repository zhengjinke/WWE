#pragma once
#include "Common.h"
class Light {
	D3DLIGHT9 light;
	
public:
	enum LIGHTTYPE
	{
		POINTLIGHT,
		DIRECTIONLIGHT,
		SPOTLIGHT,
	};
	void SetPointLight(LPDIRECT3DDEVICE9 device) {
		::ZeroMemory(&light, sizeof(light));
		light.Type = D3DLIGHT_POINT; //点光源
		light.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);	//漫反射光
		light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//镜面反射光
		light.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);	//环境光
		light.Position = D3DXVECTOR3(0.0f, 200.0f, 0.0f);	//光源位置
		//light.Direction = D3DXVECTOR3(, , );				//方向
		light.Attenuation0 = 1.0f;							//衰减  ：v = 1/(A0+A1*D+A2+D^2)
		light.Attenuation1 = 0.0f;
		light.Attenuation2 = 0.0f;
		light.Range = 300.f;								//光照范围
		device->SetLight(0, &light);
		device->LightEnable(0, true);
	}

	void SetDirectionalLight(LPDIRECT3DDEVICE9 device) {
		::ZeroMemory(&light, sizeof(light));
		light.Type = D3DLIGHT_DIRECTIONAL;					//平行光源
		light.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	//漫反射光
		light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//镜面反射光
		light.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//环境光
		//light.Position = D3DXVECTOR3(0.0f, 200.0f, 0.0f);	//光源位置
		light.Direction = D3DXVECTOR3(1.0f, 0.0f, 1.0f);	//方向
		//light.Attenuation0 = 1.0f;						//衰减  ：v = 1/(A0+A1*D+A2+D^2)
		//light.Attenuation1 = 0.0f;
		//light.Attenuation2 = 0.0f;
		//light.Range = 300.f;								//光照范围
		device->SetLight(0, &light);
		device->LightEnable(0, true);
		device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		device->SetRenderState(D3DRS_SPECULARENABLE, true);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	void SetSpotLight(LPDIRECT3DDEVICE9 device) {
		::ZeroMemory(&light, sizeof(light));
		light.Type = D3DLIGHT_SPOT;								//聚光灯
		light.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);		//漫反射光
		light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//镜面反射光
		light.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);		//环境光
		light.Position = D3DXVECTOR3(100.0f, 100.0f, 100.0f);	//光源位置
		light.Direction = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);		//方向
		light.Attenuation0 = 1.0f;								//衰减  ：v = 1/(A0+A1*D+A2+D^2)
		light.Attenuation1 = 0.0f;
		light.Attenuation2 = 0.0f;
		light.Range = 300.f;									//光照范围
		light.Falloff = 1.0f;
		light.Phi = D3DX_PI / 3.0f;
		light.Theta = D3DX_PI / 6.0f;
		device->SetLight(0, &light);
		device->LightEnable(0, true);
	}
	void SetLight(LPDIRECT3DDEVICE9 device, LIGHTTYPE type) {
		switch (type) {
		case POINTLIGHT:		SetPointLight(device);			break;
		case DIRECTIONLIGHT:	SetDirectionalLight(device);	break;
		case SPOTLIGHT:			SetSpotLight(device);			break;
		}
	}
};

