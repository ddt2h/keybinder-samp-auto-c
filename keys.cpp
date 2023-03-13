#include "Keys.h"
#include "Script.h"
bool isGameOpen()
{
	HWND gta_handle;
	gta_handle = FindWindowA(NULL, "GTA:SA:MP");
	return GetForegroundWindow() == gta_handle;
}
bool GTA_STATE;
void fastest_script()
{
	Scripter script;

	//fastest script setup
	script.setActivationButton(sf::Mouse::Button::XButton2);
	script.addButtonKeyboard(0, DIKEYBOARD_F10, 10);
	script.addButtonMouse(0, 1, 300);
	script.addButtonMouse(250, 0, 10);
	script.addButtonKeyboard(320, DIKEYBOARD_C, 10);

	int latest = 320;
	int pkm_hold = 35;

	int pkm_timing = 20;
	int lkm_timing = 40;
	int cycle_time = 90;//90 is lowest
	for (int i = 0; i < 6; i++)
	{
		script.addButtonMouse(latest + pkm_timing, 1, pkm_hold);
		script.addButtonMouse(latest + lkm_timing, 0, 10);
		script.addButtonKeyboard(latest + cycle_time, DIKEYBOARD_C, 10);


		latest = latest + cycle_time;
	}
	script.addButtonKeyboard(latest + 50, DIKEYBOARD_F10, 10);
	script.addButtonScroll(latest + 50, -1);
	script.addButtonScroll(latest + 100, 1);

	script.set_external(&GTA_STATE);
	script.start();
}
void legit_script()
{
	Scripter script;

	//fastest script setup
	script.setActivationButton(sf::Mouse::Button::XButton1);
	
	script.addButtonMouse(0, 1, 300);
	script.addButtonMouse(250, 0, 10);
	script.addButtonKeyboard(320, DIKEYBOARD_C, 10);

	int latest = 320;
	int pkm_hold = 35;

	int pkm_timing = 20;
	int lkm_timing = 40;
	int cycle_time = 215;//90 is lowest
	for (int i = 0; i < 6; i++)
	{
		script.addButtonMouse(latest + pkm_timing, 1, pkm_hold);
		script.addButtonMouse(latest + lkm_timing, 0, 10);
		script.addButtonKeyboard(latest + cycle_time, DIKEYBOARD_C, 10);


		latest = latest + cycle_time;
	}
	script.set_external(&GTA_STATE);

	script.start();
}

int main()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	std::thread fast(fastest_script);
	fast.detach();

	std::thread legit(legit_script);
	legit.detach();

	for (;;)
	{
		GTA_STATE = isGameOpen();
		
		Sleep(100);
	}
	
}
