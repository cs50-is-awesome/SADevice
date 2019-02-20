#pragma once

#define KEY_PRESSED_MASK 0x1
#define KEY_DOWN_MASK 0x8000

bool isKeyDown(int keycode);
bool wasKeyPressed(int keycode);

void initGamepad();
void gamepadFrameStart();
void gamepadFrameEnd();

bool isGamepadConnected();
bool isGamepadButtonDown(int button);
bool wasGamepadButtonPressed(int button);
bool wasGamepadButtonReleased(int button);

float getGamepadLeftStickX();
float getGamepadLeftStickY();
float getGamepadRightStickX();
float getGamepadRightStickY();