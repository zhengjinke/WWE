#pragma once
#include "Common.h"
#include "WObject.h"
class WControllerManager {
	vector<WObject*>m_controller;
	priority_queue<ObjFreeHashNode>m_qfree;
public:
	void AddController(WObject* pobj);
};
