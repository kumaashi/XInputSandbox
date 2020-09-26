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


	//Get input info.
	printf("dev-%d:pktnum:%d, btn:0x%04X, LT:%d, RT:%d, stick:%d %d, %d %d\r",
		index,
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

void xinput_ctrl_get_battery_info() {
	printf("\n\n============================\n");
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++ ) {
		printf("dev-%d:", i);

		//Get Battery info.
		XINPUT_BATTERY_INFORMATION batinfo = {};
		XInputGetBatteryInformation(i, BATTERY_DEVTYPE_GAMEPAD, &batinfo);
		if(batinfo.BatteryType == BATTERY_TYPE_DISCONNECTED) {
			printf("BATTERY_TYPE_DISCONNECTED	The device is not connected.\n");
			continue;
		}
		if(batinfo.BatteryType == BATTERY_TYPE_WIRED)
			printf("BATTERY_TYPE_WIRED	The device is a wired device and does not have a battery.\n");
		if(batinfo.BatteryType == BATTERY_TYPE_ALKALINE)
			printf("BATTERY_TYPE_ALKALINE	The device has an alkaline battery.\n");
		if(batinfo.BatteryType == BATTERY_TYPE_NIMH)
			printf("BATTERY_TYPE_NIMH	The device has a nickel metal hydride battery.\n");
		if(batinfo.BatteryType == BATTERY_TYPE_UNKNOWN)
			printf("BATTERY_TYPE_UNKNOWN	The device has an unknown battery type.\n");
	}
	printf("============================\n");
}

int main()
{
	int count = 0;
	while ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) == 0) {
		xinput_ctrl();
		count++;
		if ( (count % 60) == 0)
			xinput_ctrl_get_battery_info();

		Sleep(16);
	}
	return 0;
}

