#include "Input.h"

DInputClass::DInputClass() {
	m_pDirectInput = NULL;
	m_KeyboardDevice = NULL;
	ZeroMemory(m_keyBuffer, sizeof(char) * 256);
	m_MouseDevice = NULL;
	ZeroMemory(&m_MouseDevice, sizeof(m_MouseDevice));
}

HRESULT DInputClass::Init(HWND hwnd, HINSTANCE hInstance, DWORD keyboardCoopFlags, DWORD mouseCoopFlags) {
	HRESULT hr;
	HR(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,IID_IDirectInput8, (void**)&m_pDirectInput, NULL));

	HR(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_KeyboardDevice, NULL));
	HR(m_KeyboardDevice->SetCooperativeLevel(hwnd, keyboardCoopFlags));
	HR(m_KeyboardDevice->SetDataFormat(&c_dfDIKeyboard));
	HR(m_KeyboardDevice->Acquire());
	HR(m_KeyboardDevice->Poll());

	HR(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_MouseDevice, NULL));
	HR(m_MouseDevice->SetCooperativeLevel(hwnd, mouseCoopFlags));
	HR(m_MouseDevice->SetDataFormat(&c_dfDIMouse));
	HR(m_MouseDevice->Acquire());
	HR(m_MouseDevice->Poll());

	return S_OK;
}

void DInputClass::GetInput() {
	HRESULT hr = m_KeyboardDevice->GetDeviceState(sizeof(m_keyBuffer), (void **)&m_keyBuffer);
	if (hr) {
		m_KeyboardDevice->Acquire();
		strcpy(m_LastKeyBuffer, m_keyBuffer);
		m_KeyboardDevice->GetDeviceState(sizeof(m_keyBuffer), (LPVOID)m_keyBuffer);
	}
	hr = m_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&m_MouseState);
	if (hr) {
		m_MouseDevice->Acquire();
		m_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&m_MouseState);
	}
}

bool DInputClass::isKeyDown(int iKey) {
	if (m_keyBuffer[iKey] & 0x80)return true;
	return false;
}


bool DInputClass::isKeyUp(int iKey)
{
	if ((m_LastKeyBuffer[iKey] & 0x80) && (!(m_keyBuffer[iKey] & 0x80)))return true;
	return false;
}

bool DInputClass::IsMouseButtonDown(int button) {
	return (m_MouseState.rgbButtons[button] & 0x80)!= 0;
}

float DInputClass::MouseDx() {
	return (float)m_MouseState.lX;
}

float DInputClass::MouseDy() {
	return (float)m_MouseState.lY;
}

float DInputClass::MouseDz() {
	return (float)m_MouseState.lZ;
}

DInputClass::~DInputClass() {
	if (m_KeyboardDevice != NULL)m_KeyboardDevice->Unacquire();
	if (m_MouseDevice != NULL)m_MouseDevice->Unacquire();
	SAFE_RELEASE(m_KeyboardDevice);
	SAFE_RELEASE(m_MouseDevice);
	SAFE_RELEASE(m_pDirectInput)
}