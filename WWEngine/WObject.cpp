#include "WObject.h"


void WObject::SetIdx(unsigned idx)
{
	this->idx = idx;
}

unsigned WObject::GetIdx()
{
	return this->idx;
}

//-----------------------------------------------------------------
//	bool WObject::SetMatrix()
//	通过object的transform对象来设置开obj的世界矩阵
//-----------------------------------------------------------------
bool WTransform::SetMatrix()
{
	D3DXMatrixIdentity(&identity);
	WMATRIX matScal;
	D3DXMatrixScaling(&matScal, scal.x, scal.y, scal.z); 
	identity =identity*matScal;
	WMATRIX tmp, Rx, Ry, Rz;
	D3DXMatrixRotationX(&Rx, rotation.x);
	D3DXMatrixRotationY(&Ry, rotation.y);
	D3DXMatrixRotationZ(&Rz, rotation.z);
	identity = identity*Rx*Ry*Rz;
	D3DXMatrixTranslation(&tmp, position.x, position.y, position.z);
	identity = identity*tmp;
	return true;
}

WObject::WObject()
{
	idx = -1;
	transform.SetMatrix();
}

WObject::WObject(float x, float y, float z)
{
	idx = -1;
	transform.SetPos(x, y, z);
}

bool WObject::SetName(char * name,int nNamelen)
{
	if (nNamelen > MAX_OBJNAME) {
		messagebox("对象名超出限制", "错误");
		return false;
	}
	strncpy(m_szName, name, nNamelen * sizeof(char));
	return false;
}

void WTransform::SetPos(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	SetMatrix();
}

void WTransform::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
	SetMatrix();
}

void WTransform::SetScal(float x, float y, float z)
{
	scal.x = x;
	scal.y = y;
	scal.z = z;
	SetMatrix();
}

void WTransform::MoveAhead(float distance)
{
	position.x += identity._31*distance;
	position.y += identity._32*distance;
	position.z += identity._33*distance;
	cout << position.x << " " << position.y << " " << position.z << endl;
	SetMatrix();
}

void WTransform::MoveRight(float distance)
{
	position.x += identity._11*distance;
	position.y += identity._12*distance;
	position.z += identity._13*distance;
	SetMatrix();
}

void WTransform::MoveUp(float distance)
{
	position.x += identity._21*distance;
	position.y += identity._22*distance;
	position.z += identity._23*distance;
	SetMatrix();
}

void WTransform::Rotate(float x,float y,float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
	SetMatrix();
}
