/*
//////////////////////////////////////////////////////////////////////////

WCharactor 类
Charactor 继承 Obj,与obj显著的一个区别是，obj是world中一切事物的抽象，而charactor在world中承担着某些具体的作用。
着意味着charactor往往需要包含一个动画控制和交互系统。
它可以是一个player，也可以是一个npc。同样能是一块包含物理逻辑的石头等等。
相比obj这个更为抽象的概念，world中的事物更多的时候需要charactor来具体表现。

//////////////////////////////////////////////////////////////////////////

WCharactor 类的动画控制器
控制其应该有足够完善的底层，上层只需要在合适的时候发出切换/执行某个动作的指令即可。动作与动作之间切换的逻辑上层逻辑部分
不应该需要额外考虑。
对任何charactor而言，应该有一个默认动画实例Default。
控制器应该能够具备控制足够多动画的能力。
控制器应该能够操控动画之间进行平滑的过度。
动画控制器应该尽可能不直接参与底层接口（如d3d）的调用。

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