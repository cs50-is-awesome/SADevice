#include "d3dhook.h"

#include "dllmain.h"
#include "hooking.h"

#include <iostream>
#include <fstream>

LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;

bool match(const byte* pData, const byte* bMask, const char* szMask) {
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;
	return (*szMask) == NULL;
}

dword find_pattern(dword dwAddress, dword dwLen, byte* bMask, const char* szMask)
{
	for (dword i = 0; i < dwLen; i++)
		if (match((byte*)(dwAddress + i), bMask, szMask))
			return (dword)(dwAddress + i);
	return 0;
}

LRESULT CALLBACK myWindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void hookEndScene() {
	//HMODULE hDLL = GetModuleHandleA("d3d9");
	//LPDIRECT3D9(__stdcall*pDirect3DCreate9)(UINT) = (LPDIRECT3D9(__stdcall*)(UINT))GetProcAddress(hDLL, "Direct3DCreate9");

	WNDCLASSEX wc = { sizeof(WNDCLASSEX),CS_CLASSDC,myWindowProc,0L,0L,GetModuleHandle(NULL),NULL,NULL,NULL,NULL,("1"),NULL };
	RegisterClassEx(&wc);
	HWND hWnd = CreateWindow(("1"), NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, GetDesktopWindow(), NULL, wc.hInstance, NULL);

	if (!hWnd)
		return;

	d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

	D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information
	ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
	d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	// create a device class using this information and information from the d3dpp stuct

	if (d3d) {
		d3d->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,// | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
			&d3dpp,
			&d3ddev);
		d3d->Release();
	}
	if (hWnd)
		DestroyWindow(hWnd);
	
	std::ofstream out;
	out.open("address.txt");
	PDWORD_PTR Vtable = *reinterpret_cast<PDWORD_PTR*>(d3ddev);
	out << "0x" << std::hex << (Vtable + sizeof(void(*)())*42);
	out.close();
	//originalEndScene = (oEndScene)Vtable[42];
	//Vtable[42] = (dword)&paint;
}