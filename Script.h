#pragma once
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <thread>
#include <dinput.h>

struct ScriptedAction
{
	int key = -1;
	int mouse_key = -1;
	int mouse_scroll = 0; //so its can go bothways
	int hold_time_ms;
	bool executed = false;

	double timing;
};
class Scripter
{
private:
	double time;
	sf::Clock clock;

	std::vector<ScriptedAction> actions;

	int current_script_id = 0;

	bool isRepeated;



	sf::Keyboard::Key enable_key;
	sf::Mouse::Button enable_mouse;
	bool enable_locker;

	bool run_script;

	bool *_external; //FOR FINDING GTA HWND
	
	
	
	void keyboard_act(int key_code, int holdtime_ms) //mb set a minimum interval
	{
		INPUT ip;
		ip.type = INPUT_KEYBOARD;
		ip.ki.time = 0;
		ip.ki.wVk = 0;
		ip.ki.dwExtraInfo = 0;
		ip.ki.dwFlags = KEYEVENTF_SCANCODE;
		ip.ki.wScan = key_code;

		SendInput(1, &ip, sizeof(INPUT));
		ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		Sleep(holdtime_ms);
		SendInput(1, &ip, sizeof(INPUT));
	}

	void LeftClick(int hold_time_ms = 30)
	{
		INPUT    Input = { 0 };
		// left down 
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		::SendInput(1, &Input, sizeof(INPUT));

		
		Sleep(hold_time_ms);
		// left up
		::ZeroMemory(&Input, sizeof(INPUT));
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		::SendInput(1, &Input, sizeof(INPUT));
	}
	void RightClick(int hold_time_ms = 30)
	{
		INPUT    Input = { 0 };
		// left down 
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		::SendInput(1, &Input, sizeof(INPUT));

		
		Sleep(hold_time_ms);
		// left up
		::ZeroMemory(&Input, sizeof(INPUT));
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		::SendInput(1, &Input, sizeof(INPUT));
	}
	void MouseScroll(int RY)
	{
		INPUT in;
		in.type = INPUT_MOUSE;
		in.mi.dwFlags = MOUSEEVENTF_WHEEL;
		in.mi.mouseData = RY;
		in.mi.time = 0;
		SendInput(1, &in, sizeof(in));
	}
	void dispatch_script()
	{
		for (int i = 0; i < actions.size(); i++)
		{

			actions[i].executed = false;
		}
		time = 0; 
		clock.restart();
	}
	void _activate_state()
	{
		if ((sf::Mouse::isButtonPressed(enable_mouse)) && !enable_locker)
		{
		

			dispatch_script();
			
			run_script = true;
			enable_locker = true;

			
		}
		if (!sf::Mouse::isButtonPressed(enable_mouse))
		{
			enable_locker = false;
		}
	}
	void _abort_state()
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl))
		{
			exit(0);
		}
	} 
	
	void executeScript(int script_id) //new thread
	{
		std::cout << "script executed, " << actions[script_id].timing << std::endl;
		ScriptedAction exec = actions[script_id];

		//case keyboard key
		if (exec.key != -1)
		{	
			keyboard_act(exec.key, exec.hold_time_ms);
	     	//
		}
		if (exec.mouse_key == 0)
		{
			LeftClick(exec.hold_time_ms);
		}
		if (exec.mouse_key == 1)
		{
			RightClick(exec.hold_time_ms);
		}
		if (exec.mouse_scroll != 0)
		{
			MouseScroll(exec.mouse_scroll);
		}
		
		
		
	}
	
	void cycle()
	{
		//Sleep(1);
		_abort_state();
		_activate_state();

	
		
		if (run_script && *_external)
		{
			time = clock.getElapsedTime().asMilliseconds();
			
			if (isRepeated)
			{
				if (time >= actions[actions.size() - 1].timing + actions[actions.size() - 1].hold_time_ms)
				{
					dispatch_script();

					
				}
			} 


			for (int i = 0; i < actions.size(); i++)
			{
				if (time >= actions[i].timing && !actions[i].executed)
				{

					std::thread ex_thread(&Scripter::executeScript, this, i);
					ex_thread.detach();
					//executeScript(i);

					actions[i].executed = true;




				}
			}
		}
		
		
	}
public:
	Scripter()
	{
		
	}
	void set_external(bool *value)
	{
		_external = value;
	}
	
	void addButtonKeyboard(double timing, int key_code, int hold_time_ms)
	{
		ScriptedAction act;
		act.hold_time_ms = hold_time_ms;
		act.key = key_code;
		
		act.timing = timing;


		actions.push_back(act);
		
	}
	void addButtonMouse(double timing, int mouse_id, int hold_time_ms)
	{
		ScriptedAction act;
		act.timing = timing;
		act.hold_time_ms = hold_time_ms;
		act.mouse_key = mouse_id;
		actions.push_back(act);
	}
	void addButtonScroll(double timing, int scroll_value)
	{
		ScriptedAction act;
		act.timing = timing;
		act.mouse_scroll = scroll_value;
		actions.push_back(act);
	}
	void setActivationButton(sf::Mouse::Button key)
	{
		enable_mouse = key;
	}
	void setRepeated()
	{
		isRepeated = true;
	}
	void start()
	{
		clock.restart();
		for (;;)
		{
			cycle();
		}
	}
};