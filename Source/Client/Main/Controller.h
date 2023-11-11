#pragma once

class Controller
{
public:

	void Load(HINSTANCE hins);

private:

	static LRESULT WINAPI Mouse(int Code, WPARAM wParam, LPARAM lParam);

	static LRESULT WINAPI Keyboard(int Code, WPARAM wParam, LPARAM lParam);

	static SHORT WINAPI GetAsyncKeyStateHook(int key);

	static void CheckKeyboardKeys();

private:

	HINSTANCE Instance;

	HHOOK MouseHook;

	HHOOK KeyboardHook;

public:

	bool AutoClick;

	bool AutoClickState;
};

extern Controller gController;