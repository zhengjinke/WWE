#include "WWorld.h"

WWorld::WWorld(IDirect3DDevice9 *g_pDevice)
{
	Init(g_pDevice);
}

void WWorld::AddObj(WObject *obj)
{
	int _index = 0;
	if (m_freeque.empty()==true) {
		obj->SetIdx(m_objBuf.size());
		m_objBuf.push_back(obj);
		_index = m_objBuf.size() - 1;
	}
	else {
		int nfreeidx = m_freeque.top().idx;
		m_freeque.pop();
		obj->SetIdx(nfreeidx);
		m_objBuf[nfreeidx] = obj;
		_index = nfreeidx;
	}
	string strName = charStr2string(obj->GetName());
	cout << "strName:" << strName << endl;
	m_mapName2Index[strName] = _index;
	//DEBUG
	cout << "Load Obj " << strName << "  " << _index << endl;
}

bool WWorld::RemoveObj(int nIdx) 
{
	if (m_objBuf[nIdx] == NULL) {
		messagebox("δ�ҵ���Ҫ�Ƴ��Ķ���", "�쳣");
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


int WWorld::GetPlayerIndexByName(string szName)
{
	if (m_mapName2Index.find(szName) != m_mapName2Index.end()) {
		return m_mapName2Index[szName];
	}
	return -1;
}


WObject * WWorld::GetPlayerByName(string szName)
{
	if (m_mapName2Index.find(szName) != m_mapName2Index.end()) {
		return this->m_objBuf[m_mapName2Index[szName]];
	}
	return NULL;
}

void WWorld::SetCamera(IDirect3DDevice9* g_pDevice, ID3DXEffect* g_pEffect)
{
	//�����������������������λ����Ϣ�����ӿڱ任��ȡ���仯��
	//Ŀǰ�ò��ַ���app.render��ʵ�֡�
	m_camera->SetCamera(g_pDevice, g_pEffect);
}

void WWorld::Draw(IDirect3DDevice9*	g_pDevice)
{
	if (g_pDevice == NULL) {
		messagebox("�豸��ʧ");
		return;
	}
	for (unsigned i = 0; i < m_objBuf.size(); i++) 
	{
		if (m_objBuf[i] == NULL) continue;
		m_objBuf[i]->Draw(g_pDevice);
	}
}

void WWorld::Draw(IDirect3DDevice9*	g_pDevice,SkinnedMesh a,D3DMATRIX identity)
{
	if (g_pDevice == NULL) 
	{
		messagebox("�豸��ʧ");
		return;
	}
	a.SetPose(identity);
	a.Render(NULL);
}

void WWorld::UpdateWorld(float m_deltaTime, CallbackHandler &callbackHandler)
{
	for (unsigned i = 0; i < m_objBuf.size(); i++) 
	{
		if (m_objBuf[i] == NULL) continue;
		m_objBuf[i]->Update(m_deltaTime, callbackHandler);
	}
}

void WWorld::Init(IDirect3DDevice9 *g_pDevice)
{
	while (!m_freeque.empty())m_freeque.pop();
	for (unsigned i = 0; i < m_objBuf.size(); i++)if (m_objBuf[i]!=NULL)m_objBuf[i]->Release();
	m_mapName2Index.clear();
	m_objBuf.clear();
	m_camera = new WCamera(0.0f, 1.5f, -3.0f);
	m_pDevice = g_pDevice;
}

bool WWorld::Release()
{
	//ע�������
	m_camera->Release(); 
	delete m_camera;

	//ע������
	mFoundation->release();
	mProfileZoneManager->release();
	mPhysics->release();
	mCooking->release();
	//PxCloseExtensions();

	//ע��������еĶ���
	for (size_t i = 0; i < m_objBuf.size(); i++) {
		if (m_objBuf[i] == NULL) continue;
		m_objBuf[i]->Release();
		delete m_objBuf[i];
	}

	return true;
}

