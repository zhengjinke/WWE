#include "WCharactor.h"

WCharactor::WCharactor(char * szFile, char * szName, float x, float y, float z)
{
	m_mesh.Load(szFile);
	SetName(szName, sizeof(char)*(strlen(szName)+1));
	transform.SetPos(x, y, z);
	m_mesh.GetAnimations(animations);
	ShowAnimations();
}

void WCharactor::ShowAnimations()
{
	for (unsigned i = 0; i < animations.size(); i++) {
		std::cout <<"animation ["<<i<<"] is "<<animations[i] << endl;
	}
}

void WCharactor::Draw(IDirect3DDevice9*	g_pDevice)
{
	if (g_pDevice == NULL) {
		messagebox("Éè±¸¶ªÊ§");
		return;
	}
	m_mesh.SetPose(transform.identity);
	m_mesh.Render(NULL);
}
