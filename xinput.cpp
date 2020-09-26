//
//XInputSandbox 2020 @gyabo
//
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <algorithm>
#include <xinput.h>

void xinput_ctrl_vibration(int index, int left = 0, int right = 0)
{
	XINPUT_VIBRATION vibration = {};
	vibration.wLeftMotorSpeed = (std::max)(0, (std::min)(left, 0xFFFF));
	vibration.wRightMotorSpeed = (std::max)(0, (std::min)(right, 0xFFFF));
	XInputSetState(index, &vibration );
}

void xinput_debug(int index, XINPUT_STATE & state)
{
	auto & gpad = state.Gamepad;

	printf("pktnum:%d, btn:0x%04X, LT:%d, RT:%d, stick:%d %d, %d %d\n",
		state.dwPacketNumber,
		gpad.wButtons,
		gpad.bLeftTrigger,
		gpad.bRightTrigger,
		gpad.sThumbLX,
		gpad.sThumbLY,
		gpad.sThumbRX,
		gpad.sThumbRY);
	auto LT = gpad.bLeftTrigger;
	auto RT = gpad.bRightTrigger;
	static int count = 0;
	count++;
	if ( (count % 4) == 0)
		xinput_ctrl_vibration(index, LT * LT, RT * RT);

	if (gpad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		printf("XINPUT_GAMEPAD_DPAD_UP\n");
	if (gpad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		printf("XINPUT_GAMEPAD_DPAD_DOWN\n");
	if (gpad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		printf("XINPUT_GAMEPAD_DPAD_LEFT\n");
	if (gpad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		printf("XINPUT_GAMEPAD_DPAD_RIGHT\n");
	if (gpad.wButtons & XINPUT_GAMEPAD_START)
		printf("XINPUT_GAMEPAD_START\n");
	if (gpad.wButtons & XINPUT_GAMEPAD_BACK)
		printf("XINPUT_GAMEPAD_BACK\n");
	if (gpad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
		printf("XINPUT_GAMEPAD_LEFT_THUMB\n");
	if (gpad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
		printf("XINPUT_GAMEPAD_RIGHT_THUMB\n");
	if (gpad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		printf("XINPUT_GAMEPAD_LEFT_SHOULDER\n");
	if (gpad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		printf("XINPUT_GAMEPAD_RIGHT_SHOULDER\n");
	if (gpad.wButtons & XINPUT_GAMEPAD_A)
		printf("XINPUT_GAMEPAD_A\n");
	if (gpad.wButtons & XINPUT_GAMEPAD_B)
		printf("XINPUT_GAMEPAD_B\n");
	if (gpad.wButtons & XINPUT_GAMEPAD_X)
		printf("XINPUT_GAMEPAD_X\n");
	if (gpad.wButtons & XINPUT_GAMEPAD_Y)
		printf("XINPUT_GAMEPAD_Y\n");
}

void xinput_ctrl()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++ ) {
		XINPUT_STATE state = {};
		DWORD dwResult = XInputGetState( i, &state );
		if ( dwResult != ERROR_SUCCESS )
			continue;
		xinput_debug(i, state);
	}
}

int main()
{
	while ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) == 0) {
		xinput_ctrl();
		Sleep(16);
	}
	return 0;
}

