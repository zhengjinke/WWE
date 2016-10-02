#include "WCharactor.h"

WCharactor::WCharactor(char * szFile, char * szName, float x, float y, float z)
{
	cout << szFile << endl;
	m_mesh.Load(szFile);
	SetName(szName, sizeof(char)*(strlen(szName)+1));
	transform.SetPos(x, y, z);
	SetMatrix();
}

void WCharactor::Draw(IDirect3DDevice9*	g_pDevice)
{
	if (g_pDevice == NULL) {
		messagebox("Éè±¸¶ªÊ§");
		return;
	}
	m_mesh.SetPose(identity);
	m_mesh.Render(NULL);
}
