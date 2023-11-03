#pragma once

class Controller
{
public:

	void Load(HINSTANCE hins);

	static LRESULT WINAPI Mouse(int Code, WPARAM wParam, LPARAM lParam);

	static LRESULT WINAPI Keyboard(int Code, WPARAM wParam, LPARAM lParam);

	static SHORT WINAPI GetAsyncKeyStateHook(int key);

	static void CheckKeyboardKeys();

public:

	bool AutoClick;

	bool AutoClickState;

private:

	HHOOK MouseHook;

	HHOOK KeyboardHook;

	HINSTANCE Instance;
};

extern Controller gController;