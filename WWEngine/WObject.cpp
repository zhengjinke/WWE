#include "WObject.h"


//-----------------------------------------------------------------
//	bool WObject::SetMatrix()
//	通过object的transform对象来设置开obj的世界矩阵
//-----------------------------------------------------------------
bool WObject::SetMatrix()
{
	D3DXMatrixIdentity(&identity);
	WMATRIX matScal;
	VECTOR3 scal = transform.scal;
	D3DXMatrixScaling(&matScal, scal.x, scal.y, scal.z); 
	identity =identity*matScal;
	WMATRIX tmp, Rx, Ry, Rz;
	D3DXMatrixRotationX(&Rx, transform.rotation.x);
	D3DXMatrixRotationY(&Ry, transform.rotation.y);
	D3DXMatrixRotationZ(&Rz, transform.rotation.z);
	identity = identity*Rx*Ry*Rz;
	D3DXMatrixTranslation(&tmp, transform.position.x, transform.position.y, transform.position.z);
	identity = identity*tmp;
	return true;
}

WObject::WObject()
{
	SetMatrix();
}

WObject::WObject(float x, float y, float z)
{
	transform.position.x = x;
	transform.position.y = y;
	transform.position.z = z;
	SetMatrix();
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
}

void WTransform::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

void WTransform::SetScal(float x, float y, float z)
{
	scal.x = x;
	scal.y = y;
	scal.z = z;
}
