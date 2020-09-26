//
// XInputSandbox 2020 @gyabo
//
// ref:https://docs.microsoft.com/en-us/windows/win32/xinput/getting-started-with-xinput
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <algorithm>
#include <xinput.h>

#define INTERVAL_VIBRATION_CTRL 4
//#define _DEBUG_GAMEPAD_

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
		state.dwPacketNumber, gpad.wButtons,
		gpad.bLeftTrigger, gpad.bRightTrigger,
		gpad.sThumbLX, gpad.sThumbLY, gpad.sThumbRX, gpad.sThumbRY);

	static int count = 0;
	count++;
	if ( (count % INTERVAL_VIBRATION_CTRL) == 0) {
		auto LT = gpad.bLeftTrigger;
		auto RT = gpad.bRightTrigger;
		xinput_ctrl_vibration(index, LT * LT, RT * RT);
	}
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

