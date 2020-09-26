//
// XInputSandbox 2020 @gyabo
//
// ref:https://docs.microsoft.com/en-us/windows/win32/xinput/
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

#define INTERVAL_VIBRATION_CTRL	4

void
xinput_ctrl_vibration(int index, int left = 0, int right = 0)
{
	XINPUT_VIBRATION vibration = {};

	vibration.wLeftMotorSpeed = (std::max)(0, (std::min)(left, 0xFFFF));
	vibration.wRightMotorSpeed = (std::max)(0, (std::min)(right, 0xFFFF));
	XInputSetState(index, &vibration );
}

void
xinput_debug(int index, XINPUT_STATE & state)
{
	static int count = 0;
	auto & gpad = state.Gamepad;

	//Get input info.
	printf("dev-%d:pkt:%d, btn:0x%04X, LT:%d, RT:%d, stick:%d %d, %d %d\r",
		index, state.dwPacketNumber, gpad.wButtons,
		gpad.bLeftTrigger, gpad.bRightTrigger,
		gpad.sThumbLX, gpad.sThumbLY, gpad.sThumbRX, gpad.sThumbRY);

	count++;
	if ( (count % INTERVAL_VIBRATION_CTRL) == 0) {
		auto LT = gpad.bLeftTrigger;
		auto RT = gpad.bRightTrigger;
		xinput_ctrl_vibration(index, LT * LT, RT * RT);
	}

}

void
xinput_ctrl()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++ ) {
		XINPUT_STATE state = {};
		DWORD dwResult = XInputGetState( i, &state );
		if ( dwResult != ERROR_SUCCESS )
			continue;
		xinput_debug(i, state);
	}
}

void
xinput_ctrl_get_battery_info()
{
	printf("\n\n============================\n");
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++ ) {
		printf("dev-%d:", i);

		//Get Battery info.
		XINPUT_BATTERY_INFORMATION batinfo = {};
		XInputGetBatteryInformation(
			i, BATTERY_DEVTYPE_GAMEPAD, &batinfo);
		const char *type = "BATTERY_TYPE_DISCONNECTED";
		const char *desc = "The device is not connected.";
		printf("BatteryLevel=%d, ", batinfo.BatteryLevel);
		if(batinfo.BatteryType == BATTERY_TYPE_DISCONNECTED) {
		}
		
		if(batinfo.BatteryType == BATTERY_TYPE_WIRED) {
			type = "BATTERY_TYPE_WIRED";
			desc = "type wired device does not have a battery.";
		}

		if(batinfo.BatteryType == BATTERY_TYPE_ALKALINE) {
			type = "BATTERY_TYPE_ALKALINE";
			desc = "type alkaline battery";
		}

		if(batinfo.BatteryType == BATTERY_TYPE_NIMH) {
			type = "BATTERY_TYPE_NIMH";
			desc = "type nickel metal hydride battery.";
		}

		if(batinfo.BatteryType == BATTERY_TYPE_UNKNOWN) {
			type = "BATTERY_TYPE_UNKNOWN";
			desc = "The device has an unknown battery type.";
		}
		printf("%s\t: %s\n", type, desc);
	}
	printf("============================\n");
}

int
main()
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

