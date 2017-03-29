#include <windows.h>
#include <d3dx9.h>
#include <fstream>
#include "skinnedMesh.h"
#include "WWorld.h"
#include "WText.h"
#include <iostream>
#include "Common.h"

#include "PxPhysicsAPI.h"
#include "vehicle/PxVehicleSDK.h"

using namespace std;

//Global variables
IDirect3DDevice9*	g_pDevice = NULL; 
ID3DXSprite*		g_pSprite = NULL;
ID3DXFont*			g_pFont = NULL;
ID3DXEffect*		g_pEffect = NULL;
ofstream			g_debug("debug.txt");
float				m_show = 0.0f;
WText*				g_text = NULL;

bool KeyDown(int vk_code){return (GetAsyncKeyState(vk_code) & 0x8000) ? true : false;}
bool KeyUp(int vk_code){return (GetAsyncKeyState(vk_code) & 0x8000) ? false : true;}

class Application
{
	public:		
		Application();
		~Application();
		HRESULT Init(HINSTANCE hInstance, bool windowed);
		void Update(float deltaTime);
		void Render();
		void Cleanup();
		void Quit();

		void DeviceLost();
		void DeviceGained();

		void SetupCallback();
		void SetupCallback2();
		void InitInput(HWND hwnd, HINSTANCE hInstance);
	private:		
		DInputClass*			g_pInput;
		HWND					m_mainWindow;
		D3DPRESENT_PARAMETERS	m_present;
		bool					m_deviceLost;
		SkinnedMesh				m_drone;
		ID3DXAnimationController* m_animController;
		WWorld					m_world;
		float					m_deltaTime;
		float					m_startTime;
};



CallbackHandler callbackHandler;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
		case WM_CREATE:
			break;

		case WM_DESTROY:
			::PostQuitMessage(0);
			break;			
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    Application app;
	AllocConsole();
	SetConsoleTitle("Debug");
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r+t", stdin);
	if(FAILED(app.Init(hInstance, true)))
		return 0;
	
	MSG msg;
	memset(&msg, 0, sizeof(MSG));
	DWORD startTime = GetTickCount(); 

	while(msg.message != WM_QUIT)
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
        {	
			DWORD t = GetTickCount();
			float deltaTime = (t - startTime) * 0.001f;

			app.Update(deltaTime);
			app.Render();

			startTime = t;
        }
    }

	app.Cleanup();

    return (int)msg.wParam;
}

Application::Application()
{
}

Application::~Application()
{
	if(g_debug.good())
	{
		g_debug.close();
	}
}

HRESULT Application::Init(HINSTANCE hInstance, bool windowed)
{
	g_debug << "Application Started \n";

	//Create Window Class
	WNDCLASS wc;
	memset(&wc, 0, sizeof(WNDCLASS));
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)WndProc; 
	wc.hInstance     = hInstance;
	wc.lpszClassName = "D3DWND";

	RECT rc = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	//Register Class and Create new Window
	RegisterClass(&wc);
	m_mainWindow = ::CreateWindow("D3DWND", "Character Animation with Direct3D: Example 5.2", WS_OVERLAPPEDWINDOW, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0, 0, hInstance, 0); 
	SetCursor(NULL);
	::ShowWindow(m_mainWindow, SW_SHOW);
	::UpdateWindow(m_mainWindow);

	//Create IDirect3D9 Interface
	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

    if(d3d9 == NULL)
	{
		g_debug << "Direct3DCreate9() - FAILED \n";
		return E_FAIL;
	}

	//Check that the Device supports what we need from it
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	//Check vertex & pixelshader versions
	if(caps.VertexShaderVersion < D3DVS_VERSION(2, 0) || caps.PixelShaderVersion < D3DPS_VERSION(2, 0))
	{
		g_debug << "Warning - Your graphic card does not support vertex and pixelshaders version 2.0 \n";
	}

	//Set D3DPRESENT_PARAMETERS
	m_present.BackBufferWidth            = WINDOW_WIDTH;
	m_present.BackBufferHeight           = WINDOW_HEIGHT;
	m_present.BackBufferFormat           = D3DFMT_A8R8G8B8;
	m_present.BackBufferCount            = 2;
	m_present.MultiSampleType            = D3DMULTISAMPLE_NONE;
	m_present.MultiSampleQuality         = 0;
	m_present.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	m_present.hDeviceWindow              = m_mainWindow;
	m_present.Windowed                   = windowed;
	m_present.EnableAutoDepthStencil     = true; 
	m_present.AutoDepthStencilFormat     = D3DFMT_D24S8;
	m_present.Flags                      = 0;
	m_present.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_present.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	//Hardware Vertex Processing
	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//Create the IDirect3DDevice9
	if(FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_mainWindow, vp, &m_present, &g_pDevice)))
	{
		g_debug << "Failed to create IDirect3DDevice9 \n";
		return E_FAIL;
	}

	//Release IDirect3D9 interface
	d3d9->Release();

	//Load Application Specific resources here...
	D3DXCreateFont(g_pDevice, 50, 0, FW_BOLD, 1, false,  
				   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
				   DEFAULT_PITCH | FF_DONTCARE, "Arial", &g_pFont);

	//Create Sprite
	D3DXCreateSprite(g_pDevice, &g_pSprite);

	//Load Effect
	ID3DXBuffer *pErrorMsgs = NULL;
	HRESULT hRes = D3DXCreateEffectFromFile(g_pDevice, "resources/fx/lighting.fx", NULL, NULL, D3DXSHADER_DEBUG, NULL, &g_pEffect, &pErrorMsgs);
	
	if(FAILED(hRes) && (pErrorMsgs != NULL))		//Failed to create Effect
	{
		g_debug << (char*)pErrorMsgs->GetBufferPointer() << "\n";
		return E_FAIL;
	}
	
	m_deviceLost = false;
	m_world.Init(g_pDevice);
	//m_world.CreatePhysXBox();
	m_world.CreateCharactor("resources/meshes/soldier.x", "testSoldier", 0.0f, 0.0f, 0.0f);
	//Setup mesh
	/*m_drone.Load("resources/meshes/soldier.x");
	SetupCallback();*/

	srand(GetTickCount());
	InitInput(m_mainWindow, hInstance);
	g_text = new WText(g_pDevice);
	m_startTime = (float)GetTickCount();
	return S_OK;
}

void Application::SetupCallback()
{
	//Get Animation controller
	m_animController = m_drone.GetController();

	//Get the "Aim" Animation set
	ID3DXKeyframedAnimationSet* animSet = NULL;
	m_animController->GetAnimationSet(1, (ID3DXAnimationSet**)&animSet);

	//Compress the animation set
	ID3DXBuffer* compressedData = NULL;
	animSet->Compress(D3DXCOMPRESS_DEFAULT, 0.5f, NULL, &compressedData);

	//Create one callback key
	UINT numCallbacks = 1;
	D3DXKEY_CALLBACK keys[1];

	// GetSourceTicksPerSecond() returns the number
	// animation key frame ticks that occur per second.
	// Callback keyframe times are tick based.
	double ticks = animSet->GetSourceTicksPerSecond();

	// Set the first callback key to trigger a callback
	// half way through the animation sequence.
	keys[0].Time = float(animSet->GetPeriod()/2.0f*ticks);
	keys[0].pCallbackData = (void*)&m_drone;

	// Create the ID3DXCompressedAnimationSet interface
	// with the callback keys.
	ID3DXCompressedAnimationSet* compressedAnimSet = NULL;
	D3DXCreateCompressedAnimationSet(animSet->GetName(), 
									 animSet->GetSourceTicksPerSecond(),
									 animSet->GetPlaybackType(), compressedData,
									 numCallbacks, keys, &compressedAnimSet);
	compressedData->Release();

	//Delete the old keyframed animation set.
	m_animController->UnregisterAnimationSet(animSet);

	// and then add the new compressed animation set.
	m_animController->RegisterAnimationSet(compressedAnimSet);

	// Hook up the animation set to the first track.
	m_animController->SetTrackAnimationSet(0, compressedAnimSet);
	compressedAnimSet->Release();
}

void Application::DeviceLost()
{
	try
	{
		g_pFont->OnLostDevice();
		g_pSprite->OnLostDevice();
		g_pEffect->OnLostDevice();
		g_text->OnLostDevice();
		m_deviceLost = true;
	}
	catch(...)
	{
		g_debug << "Error occured in Application::DeviceLost() \n";
	}
}

void Application::DeviceGained()
{
	try
	{
		g_pDevice->Reset(&m_present);
		g_pFont->OnResetDevice();
		g_pSprite->OnResetDevice();	
		g_pEffect->OnResetDevice();
		g_text->OnResetDevice();
		m_deviceLost = false;
	}
	catch(...)
	{
		g_debug << "Error occured in Application::DeviceGained() \n";
	}
}

void Application::Update(float deltaTime)
{
	try
	{
		//Check for lost device
		g_pInput->GetInput();
		HRESULT coop = g_pDevice->TestCooperativeLevel();

		if(coop != D3D_OK)
		{
			if(coop == D3DERR_DEVICELOST)
			{
				if(m_deviceLost == false)
					DeviceLost();		
			}
			else if(coop == D3DERR_DEVICENOTRESET)
			{
				if(m_deviceLost == true)
					DeviceGained();
			}

			Sleep(100);
			return;
		}

		m_deltaTime = deltaTime * 0.4f;

		if(m_show > 0.0f)
		{
			m_show -= deltaTime;
		}


		//Keyboard input
		if(KeyDown(VK_ESCAPE))
		{
			Quit();
		}

		if (g_pInput->isKeyDown(DIK_A))		m_world.m_camera->transform.MoveRight(-0.00205f);
		if (g_pInput->isKeyDown(DIK_D))		m_world.m_camera->transform.MoveRight(0.00205f);
		if (g_pInput->isKeyDown(DIK_SPACE))	m_world.m_camera->transform.MoveUp(0.00505f);
		if (g_pInput->isKeyDown(DIK_C))		m_world.m_camera->transform.MoveUp(-0.00505f);
		//if (g_pInput->isKeyDown(DIK_W))		m_world.m_camera->transform.MoveAhead(0.00505f);
		if (g_pInput->isKeyDown(DIK_S))		m_world.m_camera->transform.MoveAhead(-0.00505f);
		if (g_pInput->isKeyDown(DIK_P))
			g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		if (g_pInput->isKeyDown(DIK_O))
			g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		if (g_pInput->IsMouseButtonDown(1)) {
			m_world.m_camera->transform.Rotate(-(g_pInput->MouseDy())*-0.0021f, 0, 0);
			m_world.m_camera->transform.Rotate(0, -(g_pInput->MouseDx())*-0.0021f, 0);
		}
		if (g_pInput->IsMouseButtonDown(0)) {
			WObject* gotobj = m_world.GetPlayerByName("testSoldier");
			if (gotobj != NULL) {
				WCharactor* player = (WCharactor*)gotobj;
				player->PlayAnimation("Aim");
			}
		}
		if (g_pInput->isKeyDown(DIK_W)) {
			WObject* gotobj = m_world.GetPlayerByName("testSoldier");
			if (gotobj != NULL) {
				WCharactor* player = (WCharactor*)gotobj;
				player->PlayAnimation("Walk");
			}
		}
		/*if (g_pInput->isKeyUp(DIK_W)) {
			WObject* gotobj = m_world.GetPlayerByName("testSoldier");
			if (gotobj != NULL) {
				WCharactor* player = (WCharactor*)gotobj;
				player->PlayAnimation("Still");
			}
		}*/


		if(KeyDown(VK_RETURN) && KeyDown(18))		//ALT + RETURN
		{
			//Switch between windowed mode and fullscreen mode
			m_present.Windowed = !m_present.Windowed;

			DeviceLost();
			DeviceGained();

			if(m_present.Windowed)
			{
				RECT rc = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
				AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
				SetWindowPos(m_mainWindow, HWND_NOTOPMOST, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
				UpdateWindow(m_mainWindow);
			}
		}
	}
	catch(...)
	{
		g_debug << "Error in Application::Update() \n";
	}
}	

void Application::Render()
{
	if(!m_deviceLost)
	{
		try
		{
			m_world.SetCamera(g_pDevice, g_pEffect);
			D3DXVECTOR4 lightPos(-50.0f, 75.0f, -150.0f, 0.0f);
			//Create Matrices
			/*D3DXMATRIX identity, world, view, proj;
			D3DXMatrixIdentity(&identity);
			D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0.0f, 1.5f, -3.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 1000.0f);*/
			//m_world.m_camera->matView*m_world.m_camera->proj
			//g_pDevice->SetTransform(D3DTS_WORLD, &identity);
			g_pDevice->SetTransform(D3DTS_VIEW, &m_world.m_camera->matView);
			g_pDevice->SetTransform(D3DTS_PROJECTION, &m_world.m_camera->proj);

			// Clear the viewport
			g_pDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0L);
			
			// Begin the scene 
			if(SUCCEEDED(g_pDevice->BeginScene()))
			{	
				{
					g_pEffect->SetMatrix("matVP", &(m_world.m_camera->matView*m_world.m_camera->proj));
					g_pEffect->SetVector("lightPos", &lightPos);
					m_world.UpdateWorld(m_deltaTime, callbackHandler);
					m_world.Draw(g_pDevice);
					//g_pEffect->SetMatrix("matW", &identity);
					//g_pEffect->SetMatrix("matVP", &(view * proj));
					//m_animController->AdvanceTime(m_deltaTime, &callbackHandler);
					//m_drone.SetPose(identity);
					//m_drone.Render(NULL);
				}
				char s[50];
				sprintf(s, " %.0f\n\n %.0f", (GetTickCount()-m_startTime)/1000, (GetTickCount() - m_startTime) / 1000);
				g_text->Write(0, 0, 200, 200, s, 1);
				if(m_show > 0.0f)
				{
					RECT rc = {0, 0, 800, 600};
					g_pFont->DrawText(NULL, "BANG!", -1, &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, 0xFF000000);
					SetRect(&rc, -5, -5, 795, 595);
					g_pFont->DrawText(NULL, "BANG!", -1, &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, 0xFFFFFF00);
				}

				// End the scene.
				g_pDevice->EndScene();
				g_pDevice->Present(0, 0, 0, 0);
			}
		}
		catch(...)
		{
			g_debug << "Error in Application::Render() \n";
		}
	}
}

void Application::Cleanup()
{
	//Release all resources here...
	if(g_pSprite != NULL)		g_pSprite->Release();
	if(g_pFont != NULL)		g_pFont->Release();
	if(g_pDevice != NULL)		g_pDevice->Release();

	g_debug << "Application Terminated \n";
}

void Application::Quit()
{
	::DestroyWindow(m_mainWindow);
	::PostQuitMessage(0);
}

VOID Application::InitInput(HWND hwnd, HINSTANCE hInstance)
{
	g_pInput = new DInputClass();
	g_pInput->Init(hwnd, hInstance, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	return VOID();
}
