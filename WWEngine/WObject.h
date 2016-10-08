#pragma once
#include "Common.h"
class WTransform {
public:
	VECTOR3		position;
	VECTOR3		rotation;
	VECTOR3		scal;
	WTransform() {
		position.x = position.y = position.z = 0.0f;
		rotation.x = rotation.y = rotation.z = 0.0f;
		scal.x = scal.y = scal.z = 1.0f;
	}
	void SetPos(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScal(float x, float y, float z);
};

class WObject {
private:
	unsigned 		idx;
	char			m_szName[MAX_OBJNAME];
public:
	WMATRIX			identity;			// Object世界矩阵
	WTransform		transform;
	
	//virtual bool Create() = 0;
	void SetIdx(unsigned idx);
	unsigned GetIdx();

	bool SetMatrix();
	WObject();
	WObject(float x,float y,float z);	// 创建对象时设置position
	bool Release() { return true; }
	bool SetName(char *name,int nNamelen);




	//-------------------------------------------------------------------
	//								虚函数
	//-------------------------------------------------------------------
	virtual void Draw(IDirect3DDevice9*	g_pDevice) = 0;			// Object纯虚函数
};