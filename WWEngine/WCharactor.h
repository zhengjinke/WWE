/*
//////////////////////////////////////////////////////////////////////////

WCharactor ��
Charactor �̳� Obj,��obj������һ�������ǣ�obj��world��һ������ĳ��󣬶�charactor��world�ге���ĳЩ��������á�
����ζ��charactor������Ҫ����һ���������ƺͽ���ϵͳ��
��������һ��player��Ҳ������һ��npc��ͬ������һ����������߼���ʯͷ�ȵȡ�
���obj�����Ϊ����ĸ��world�е���������ʱ����Ҫcharactor��������֡�

//////////////////////////////////////////////////////////////////////////

WCharactor ��Ķ���������
������Ӧ�����㹻���Ƶĵײ㣬�ϲ�ֻ��Ҫ�ں��ʵ�ʱ�򷢳��л�/ִ��ĳ��������ָ��ɡ������붯��֮���л����߼��ϲ��߼�����
��Ӧ����Ҫ���⿼�ǡ�
���κ�charactor���ԣ�Ӧ����һ��Ĭ�϶���ʵ��Default��
������Ӧ���ܹ��߱������㹻�ද����������
������Ӧ���ܹ��ٿض���֮�����ƽ���Ĺ��ȡ�
����������Ӧ�þ����ܲ�ֱ�Ӳ���ײ�ӿڣ���d3d���ĵ��á�

*/
#pragma once
#include "WObject.h"
#include "skinnedMesh.h"
#include "Common.h"
class WCharactor : public WObject{
public:
	SkinnedMesh						m_mesh;
	ID3DXAnimationController *		m_aniController;
	map<string,int>					animations;		// assign when load mesh in Constructor;

	//WCharactor();
	WCharactor(char *szFile, char *szName, float x, float y, float z);
	void PlayAnimation(char szName[]);
	void ShowAnimations();
	virtual void Draw(IDirect3DDevice9*	g_pDevice);
	void SetupCallBack(unsigned int);
	virtual void Update(float m_deltaTime, CallbackHandler &callbackHandler);
	void SetType(ObjectType type);
};