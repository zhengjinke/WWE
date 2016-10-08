#include "WControllerManager.h"

void WControllerManager::AddController(WObject* pobj)
{
	if (m_qfree.empty()) {
		m_controller.push_back(pobj);
	}
}
