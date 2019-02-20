#include "gameinput.h"

#include <Windows.h>
#include <XInput.h>
#pragma comment(lib, "XInput.lib")

bool isKeyDown(int keycode)
{
	return GetAsyncKeyState(keycode) & KEY_DOWN_MASK;
}

bool wasKeyPressed(int keycode)
{
	return GetAsyncKeyState(keycode) & KEY_PRESSED_MASK;
}

XINPUT_STATE state;
WORD buttonsHeld, buttonsPressed, buttonsReleased, previousState;
bool gamepadConnected;

void gamepadFrameStart()
{
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	gamepadConnected = XInputGetState(0, &state) == ERROR_SUCCESS;
	buttonsHeld = state.Gamepad.wButtons;
	buttonsPressed = (~previousState) & state.Gamepad.wButtons;
	buttonsReleased = previousState & (~state.Gamepad.wButtons);
}

void gamepadFrameEnd()
{
	previousState = state.Gamepad.wButtons;
}

bool isGamepadConnected()
{
	return gamepadConnected;
}

bool isGamepadButtonDown(int button)
{
	return buttonsHeld & button;
}

bool wasGamepadButtonPressed(int button)
{
	return buttonsPressed & button;
}

bool wasGamepadButtonReleased(int button)
{
	return buttonsReleased & button;
}