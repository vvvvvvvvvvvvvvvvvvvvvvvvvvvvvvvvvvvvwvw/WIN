
module;
#include "pch.h"

export module eargs;
import TypeDef;

export namespace win::ui
{
	using Sender = void*;
	struct WindowEventArgs
	{
		HWND wnd;
		UINT msg;
		WPARAM wParam;
		LPARAM lParam;

		WindowEventArgs() = default;
		WindowEventArgs(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
			: wnd(wnd), msg(msg), wParam(wParam), lParam(lParam) {
		}
	};
	struct MouseEventArgs : WindowEventArgs
	{
		int x;
		int y;
		int g_x;
		int g_y;

		MouseEventArgs(int x, int y, int g_x, int g_y)
			: x(x), y(y), g_x(g_x), g_y(g_y) {}
	};
	struct PaintEventArgs : WindowEventArgs
	{
		HDC hdc;
		Rect rect;
		PaintEventArgs(HDC hdc, Rect rect)
			: hdc(hdc), rect(rect) {}
	};
}