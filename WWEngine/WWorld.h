#pragma once
#include <vector>
#include <queue>
#include "Common.h"
#include "WObject.h"
#include "WCharactor.h"
class WWorld {
	
	std::vector< WObject* >m_objBuf;					// 世界中的对象指针池
	std::priority_queue< ObjFreeHashNode >m_freeque;	// 用于查询对象指针池中是否存在被标记为空的位置
public:
	WWorld();
	void AddObj(WObject *obj);							// 将一个对象指针加入对象指针池中
	bool RemoveObj(int nIdx);
	bool CreateCharactor(char *szFile, char *szName, float x, float y, float z);
	void SetCamera();
	void Draw(IDirect3DDevice9*	g_pDevice);
	void Draw(IDirect3DDevice9*	g_pDevice, SkinnedMesh a, D3DMATRIX identity);
	void Init();
};