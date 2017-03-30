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
		light.Type = D3DLIGHT_POINT; //���Դ
		light.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);	//�������
		light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//���淴���
		light.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);	//������
		light.Position = D3DXVECTOR3(0.0f, 200.0f, 0.0f);	//��Դλ��
		//light.Direction = D3DXVECTOR3(, , );				//����
		light.Attenuation0 = 1.0f;							//˥��  ��v = 1/(A0+A1*D+A2+D^2)
		light.Attenuation1 = 0.0f;
		light.Attenuation2 = 0.0f;
		light.Range = 300.f;								//���շ�Χ
		device->SetLight(0, &light);
		device->LightEnable(0, true);
	}

	void SetDirectionalLight(LPDIRECT3DDEVICE9 device) {
		::ZeroMemory(&light, sizeof(light));
		light.Type = D3DLIGHT_DIRECTIONAL;					//ƽ�й�Դ
		light.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	//�������
		light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//���淴���
		light.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//������
		//light.Position = D3DXVECTOR3(0.0f, 200.0f, 0.0f);	//��Դλ��
		light.Direction = D3DXVECTOR3(1.0f, 0.0f, 1.0f);	//����
		//light.Attenuation0 = 1.0f;						//˥��  ��v = 1/(A0+A1*D+A2+D^2)
		//light.Attenuation1 = 0.0f;
		//light.Attenuation2 = 0.0f;
		//light.Range = 300.f;								//���շ�Χ
		device->SetLight(0, &light);
		device->LightEnable(0, true);
		device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		device->SetRenderState(D3DRS_SPECULARENABLE, true);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	void SetSpotLight(LPDIRECT3DDEVICE9 device) {
		::ZeroMemory(&light, sizeof(light));
		light.Type = D3DLIGHT_SPOT;								//�۹��
		light.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);		//�������
		light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//���淴���
		light.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);		//������
		light.Position = D3DXVECTOR3(100.0f, 100.0f, 100.0f);	//��Դλ��
		light.Direction = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);		//����
		light.Attenuation0 = 1.0f;								//˥��  ��v = 1/(A0+A1*D+A2+D^2)
		light.Attenuation1 = 0.0f;
		light.Attenuation2 = 0.0f;
		light.Range = 300.f;									//���շ�Χ
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

