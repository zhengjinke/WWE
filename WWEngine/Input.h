#pragma once
#include "Common.h"
#include <Windows.h>
#include <dinput.h>
#include <iostream>
using namespace std;
#pragma comment(lib, "dinput8")
#pragma comment(lib, "dxguid")
class DInputClass {
private:
	IDirectInput8*			m_pDirectInput;
	IDirectInputDevice8*	m_KeyboardDevice;
	char					m_keyBuffer[256];
	char					m_LastKeyBuffer[256];
	DIMOUSESTATE			m_MouseState;
	IDirectInputDevice8*	m_MouseDevice;
public :
	HRESULT Init(HWND hwnd,HINSTANCE hInstance, DWORD keyboardCoopFlags,DWORD mouseCoopFlags);
	void GetInput();
	bool isKeyDown(int iKey);
	bool isKeyUp(int iKey);
	bool IsMouseButtonDown(int button);
	float MouseDx();
	float MouseDy();
	float MouseDz();
public :
	DInputClass(void);
	~DInputClass(void);
};