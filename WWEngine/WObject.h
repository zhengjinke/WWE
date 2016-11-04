#pragma once
#include "Common.h"
class WTransform {
public:
	VECTOR3		position;
	VECTOR3		rotation;
	VECTOR3		scal;
	WMATRIX		identity;			// Object世界矩阵
									/*	identity{		物体世界矩阵含义
									_11  _12  _13  0  x方向向量，默认右侧
									_21  _22  _23  0  y方向向量，默认上方
									_31  _32  _33  0  z方向向量，默认前方
									_41  _42  _43  0  物体世界坐标
									}									*/
	bool SetMatrix();
	WTransform() {
		position.x = position.y = position.z = 0.0f;
		rotation.x = rotation.y = rotation.z = 0.0f;
		scal.x = scal.y = scal.z = 1.0f;
		SetMatrix();
	}
	void SetPos(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScal(float x, float y, float z);
	void MoveAhead(float distance);
	void MoveRight(float distance);
	void MoveUp(float distance);
	void Rotate(float x, float y, float z);
};



class WObject {
private:
	unsigned 		idx;
	char			m_szName[MAX_OBJNAME];
public:
	
	WTransform		transform;
	void SetIdx(unsigned idx);
	unsigned GetIdx();
	WObject();
	WObject(float x,float y,float z);	// 创建对象时设置position
	bool Release() { return true; }
	bool SetName(char *name,int nNamelen);
	char* GetName() { return m_szName; }
	//-------------------------------------------------------------------
	//								虚函数
	//-------------------------------------------------------------------
	virtual void Draw(IDirect3DDevice9*	g_pDevice) = 0;			// Object纯虚函数
	virtual void Update(float m_deltaTime, CallbackHandler &callbackHandler) = 0;
};