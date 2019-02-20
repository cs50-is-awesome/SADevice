#include "dllmain.h"

#include "gameinput.h"
#include "gta.h"
#include "hooking.h"
#include "types.h"

#define TARGET_FUNCTION_CALL	 0x0053C096
#define TARGET_FUNCTION_ADDRESS	 0x005684A0
void(*targetFunction)() = (void(*)()) TARGET_FUNCTION_ADDRESS;

HWND gameWindow;
bool d3dhooked;

FunctionCallHook fchMainHook((dword*)0x0053C096, (dword)&hook);

// DLL entry point - called when the DLL is loaded
BOOL WINAPI DllMain(__in  HINSTANCE hinstDLL, __in  DWORD fdwReason, __in  LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		// Call the init function once the DLL is attached to a process
		init();
	}
	return TRUE;
}

// Set the cheat device up
void init()
{
	// Find the game's window
	gameWindow = FindWindow(NULL, "GTA: San Andreas");
	// The game window doesn't exist, exit
	if (!gameWindow)
		return;

	d3dhooked = false;
	fchMainHook.install();

}

// Function the game will call every frame after init is called
void hook()
{
	// Call the hooked function to let the game update the frame 
	targetFunction();
	// Call hack to apply changes
	hack();
}

Pedestrian player;
Vehicle car;
// Separate function for all 'hacking' to take place, once per frame
void hack()
{
	if (!d3dhooked) {
		d3dhooked = true;
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)hookEndScene, 0, 0, 0);
	}
	gamepadFrameStart();

	// Get player's current address in memory and recalculate pointers if it has changed
	player.updateAddress(*(dword*)(0x00B6F5F0));

	// Get player's car's address in memory and recalculate pointers if it changed
	car.updateAddress(*(dword*)(0x00BA18FC));

	// Check to see if player's pedestrian exists (if it doesn't, probably shouldn't touch anything yet)
	if (player.isValid()) {
		// Give player infinite health and armor
		*player.health = 1000.0f;
		*player.armor = 1000.0f;

		/*if (*playerMoney < 10000000)
			*playerMoney = 10000000;*/

		// Check to see if player is in a car
		if (car.isValid()) {
			// Give car infinite health
			*car.health = 10000.0f;
		}
	}

	// Unload the DLL when the F12 key is pressed
	if (wasKeyPressed(VK_F12))
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)unload, 0, 0, 0);
	gamepadFrameEnd();
}

// Unhooks the hack and unloads the DLL from memory
void unload() {

	fchMainHook.uninstall();

	// Unload the DLL
	HMODULE hmodule;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, "SADevice.dll", &hmodule);
	FreeLibraryAndExitThread(hmodule, 0);
}

LPD3DXFONT pFont;

VOID WriteText(LPDIRECT3DDEVICE9 pDevice, INT x, INT y, DWORD color, const char* text)
{
	RECT rect;
	SetRect(&rect, x, y, x, y);
	pFont->DrawText(NULL, text, -1, &rect, DT_NOCLIP | DT_LEFT, color);
}

VOID WINAPI paint(LPDIRECT3DDEVICE9 pDevice) {
	if (pFont)
	{
		pFont->Release();
		pFont = NULL;
	}
	if (!pFont)
		D3DXCreateFont(pDevice, 14, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &pFont);
	if (pFont)
		WriteText(pDevice, 15, 15, D3DCOLOR_ARGB(255, 255, 000, 000), "HELLO WORLD");
	originalEndScene(pDevice);
}