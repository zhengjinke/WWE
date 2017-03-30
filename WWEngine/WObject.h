#pragma once
#include "Common.h"
class WTransform {
public:
	VECTOR3		position;
	VECTOR3		rotation;
	VECTOR3		scal;
	WMATRIX		identity;			// Object�������
									/*	identity{		�������������
									_11  _12  _13  0  x����������Ĭ���Ҳ�
									_21  _22  _23  0  y����������Ĭ���Ϸ�
									_31  _32  _33  0  z����������Ĭ��ǰ��
									_41  _42  _43  0  ������������
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

enum ObjectType {
	Charactor		= 0,
	StaticObject	= 1,
	Camera			= 2,
//--------------------------------
	typecount,
};

class WObject {
private:
	unsigned 		idx;
	char			m_szName[MAX_OBJNAME];
	
public:
	int				type;
	WTransform		transform;
public:
	WObject();
	WObject(float x, float y, float z);	// ��������ʱ����position

	void		SetIdx(unsigned idx);
	unsigned	GetIdx();
	bool		Release() { return true; }
	bool		SetName(char *name,int nNamelen);
	char*		GetName() { return m_szName; }
	int			GetType() { return type; }
	//-------------------------------------------------------------------
	//								�麯��
	//-------------------------------------------------------------------
	virtual void Draw(IDirect3DDevice9*	g_pDevice) = 0;			// Object���麯��
	virtual void Update(float m_deltaTime, CallbackHandler &callbackHandler) = 0;
	
	virtual void SetType(ObjectType type) = 0;
};