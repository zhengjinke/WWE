#include "WWorld.h"

WWorld::WWorld()
{
	Init();
}

void WWorld::AddObj(WObject *obj)
{
	if (m_freeque.empty()==true) {
		obj->SetIdx(m_objBuf.size());
		m_objBuf.push_back(obj);
	}
	else {
		int nfreeidx = m_freeque.top().idx;
		m_freeque.pop();
		obj->SetIdx(nfreeidx);
		m_objBuf[nfreeidx] = obj;
	}
}

bool WWorld::RemoveObj(int nIdx) 
{
	if (m_objBuf[nIdx] == NULL) {
		messagebox("未找到需要移除的对象。", "异常");
		return false;
	}
	m_objBuf[nIdx]->Release();
	m_objBuf[nIdx] = NULL;
	m_freeque.push(ObjFreeHashNode(nIdx));
	return true;
}

bool WWorld::CreateCharactor(char * szFile, char * szName, float x, float y, float z)
{
	WCharactor *pNewCharactor = new WCharactor(szFile, szName, x, y, z);
	AddObj(pNewCharactor);
	return false;
}

void WWorld::SetCamera(IDirect3DDevice9* g_pDevice, ID3DXEffect* g_pEffect)
{
	//完成摄像机类后，用摄像机对象的位置信息来做视口变换和取景变化。
	//目前该部分放在app.render中实现。
	m_camera->SetCamera(g_pDevice, g_pEffect);
}

void WWorld::Draw(IDirect3DDevice9*	g_pDevice)
{
	if (g_pDevice == NULL) {
		messagebox("设备丢失");
		return;
	}
	for (unsigned i = 0; i < m_objBuf.size(); i++) {
		if (m_objBuf[i] == NULL) continue;
		m_objBuf[i]->Draw(g_pDevice);
	}
}

void WWorld::Draw(IDirect3DDevice9*	g_pDevice,SkinnedMesh a,D3DMATRIX identity)
{
	if (g_pDevice == NULL) {
		messagebox("设备丢失");
		return;
	}
	a.SetPose(identity);
	a.Render(NULL);
}

void WWorld::UpdateWorld(float m_deltaTime, CallbackHandler &callbackHandler)
{
	for (unsigned i = 0; i < m_objBuf.size(); i++) {
		if (m_objBuf[i] == NULL) continue;
		m_objBuf[i]->Update(m_deltaTime, callbackHandler);
	}
}

void WWorld::Init()
{
	while (!m_freeque.empty())m_freeque.pop();
	for (unsigned i = 0; i < m_objBuf.size(); i++)if (m_objBuf[i]!=NULL)m_objBuf[i]->Release();
	m_objBuf.clear();
	m_camera = new WCamera(0.0f, 1.5f, -3.0f);
}

bool WWorld::Release()
{
	m_camera->Release(); delete m_camera;
	return true;
}

