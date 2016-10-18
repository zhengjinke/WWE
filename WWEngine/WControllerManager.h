/*
面向单机的控制中心
//////////////////////////////////////////////////////////////////////////
由于是面向单机，该控制器适用于默认只有一个终端的情况下。
所有外部控制器输入信息由该组件接收。然后统一分发给需要控制的模块。
所有需要控制的模块通过注册的方式添加到该控制中心。
//////////////////////////////////////////////////////////////////////////
*/

#pragma once
#include "Common.h"
#include "WObject.h"
class WControllerManager {
	vector<WObject*>m_controller;
	priority_queue<ObjFreeHashNode>m_qfree;
public:
	void AddController(WObject* pobj);
};
