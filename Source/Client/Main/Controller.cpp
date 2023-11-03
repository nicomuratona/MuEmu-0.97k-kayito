#include "stdafx.h"
#include "Camera.h"
#include "Controller.h"
#include "MiniMap.h"
#include "MoveList.h"
#include "resource.h"
#include "TrayMode.h"

#include "Sound.h"

Controller gController;

void Controller::Load(HINSTANCE hins)
{
	this->Instance = hins;

	if (!this->MouseHook)
	{
		this->MouseHook = SetWindowsHookEx(WH_MOUSE, this->Mouse, this->Instance, GetCurrentThreadId());

		if (!this->MouseHook)
		{
			return;
		}
	}

	if (!this->KeyboardHook)
	{
		this->KeyboardHook = SetWindowsHookEx(WH_KEYBOARD, this->Keyboard, this->Instance, GetCurrentThreadId());

		if (!this->KeyboardHook)
		{
			return;
		}
	}

	this->AutoClick = false;

	this->AutoClickState = false;

	SetDword(0x00552398, (DWORD)&this->GetAsyncKeyStateHook);

	SetCompleteHook(0xE8, 0x005259DD, &this->CheckKeyboardKeys);
}

LRESULT Controller::Mouse(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		MOUSEHOOKSTRUCTEX* HookStruct = (MOUSEHOOKSTRUCTEX*)lParam;

		switch (wParam)
		{
			case WM_MOUSEMOVE:
			{
				if (GetForegroundWindow() == g_hWnd)
				{
					gCamera.Move(HookStruct);
				}

				break;
			}

			case WM_MBUTTONDOWN:
			{
				if (GetForegroundWindow() == g_hWnd)
				{
					gCamera.SetIsMove(1);

					gCamera.SetCursorX(HookStruct->pt.x);

					gCamera.SetCursorY(HookStruct->pt.y);
				}

				break;
			}

			case WM_MBUTTONUP:
			{
				if (GetForegroundWindow() == g_hWnd)
				{
					gCamera.SetIsMove(0);
				}

				break;
			}

			case WM_MOUSEWHEEL:
			{
				if (GetForegroundWindow() == g_hWnd)
				{
					gCamera.Zoom(HookStruct);
				}

				break;
			}

			default:
			{
				break;
			}
		}
	}

	return CallNextHookEx(gController.MouseHook, nCode, wParam, lParam);
}

int SoundLevel = 9;

LRESULT Controller::Keyboard(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		if (((DWORD)lParam & (1 << 30)) != 0 && ((DWORD)lParam & (1 << 31)) != 0)
		{
			switch (wParam)
			{
				case VK_F9:
				{
					if (GetForegroundWindow() == g_hWnd)
					{
						gController.AutoClick ^= 1;

						if (gController.AutoClick)
						{
							SetTimer(g_hWnd, WM_AUTOCLICKTIMER, 100, 0);
						}
						else
						{
							KillTimer(g_hWnd, WM_AUTOCLICKTIMER);

							gController.AutoClickState = false;

							SendMessage(g_hWnd, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(MouseX, MouseY));
						}

						CreateNotice(((gController.AutoClick) ? "Autoclick Enabled" : "Autoclick Disabled"), 1);
					}

					break;
				}

				case VK_F10:
				{
					if (GetForegroundWindow() == g_hWnd)
					{
						gCamera.Toggle();
					}

					break;
				}

				case VK_F11:
				{
					if (GetForegroundWindow() == g_hWnd)
					{
						gCamera.Restore();
					}

					break;
				}

				case VK_F12:
				{
					if (GetForegroundWindow() == g_hWnd)
					{
						gTrayMode.Toggle();
					}

					break;
				}

				default:
				{
					break;
				}
			}
		}
	}

	return CallNextHookEx(gController.KeyboardHook, nCode, wParam, lParam);
}

SHORT WINAPI Controller::GetAsyncKeyStateHook(int key)
{
	if (GetForegroundWindow() != g_hWnd)
	{
		return 0;
	}

	return GetAsyncKeyState(key);
}

void Controller::CheckKeyboardKeys()
{
	if (((gController.GetAsyncKeyStateHook('M') >> 8) & 0x80) == 0x80) // M Pressed
	{
		if (!*(int*)(KeyState + 'M' * 4))
		{
			*(int*)(KeyState + 'M' * 4) = 1;

			gMoveList.Toggle();
		}
	}
	else
	{
		*(int*)(KeyState + 'M' * 4) = 0;
	}

	if (((gController.GetAsyncKeyStateHook(VK_TAB) >> 8) & 0x80) == 0x80) // Tab Pressed
	{
		if (!*(int*)(KeyState + VK_TAB * 4))
		{
			*(int*)(KeyState + VK_TAB * 4) = 1;

			gMiniMap.Toggle();
		}
	}
	else
	{
		*(int*)(KeyState + VK_TAB * 4) = 0;
	}

	if (((gController.GetAsyncKeyStateHook(VK_SPACE) >> 8) & 0x80) == 0x80) // Space Pressed
	{
		if (!*(int*)(KeyState + VK_SPACE * 4))
		{
			*(int*)(KeyState + VK_SPACE * 4) = 1;

			if (!InputEnable && !MouseOnWindow)
			{
				for (int i = 0; i < 1000; ++i)
				{
					DWORD ItemObject = (0x07E127F8 + 516 * i) + 72; // Items[i].Object

					if (*(BYTE*)(ItemObject + 0) // ItemObject->Live
					    && *(BYTE*)(ItemObject + 352)) // ItemObject->Visible
					{
						vec3_t vDir;

						VectorSubtract((float*)(ItemObject + 16), // ItemObject->Position
							       (float*)(Hero + 16), // Hero->Object.Position
							       vDir);

						if (sqrt(vDir[0] * vDir[0] + vDir[1] * vDir[1] + vDir[2] * vDir[2]) <= 300.0f)
						{
							*(BYTE*)(Hero + 749) = 1; // Hero->MovementType = MOVEMENT_GET;

							ItemKey = i;

							Action(Hero, ItemObject);

							*(BYTE*)(Hero + 749) = 0; // Hero->MovementType = MOVEMENT_MOVE;

							break;
						}
					}
				}
			}
		}
	}
	else
	{
		*(int*)(KeyState + VK_SPACE * 4) = 0;
	}

	((void(__cdecl*)()) 0x004C04A0)(); // Original Function
}