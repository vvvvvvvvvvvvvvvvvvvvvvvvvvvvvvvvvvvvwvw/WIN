
module;
#include "pch.h"

export module Control;

import win;
import eargs;
import Event;


export namespace win::ui
{
	class Control : public Object<Control>
	{
	protected:
		HWND m_hWnd;
		HWND m_parent;
		bool m_enabled;
		String m_name;
		Point m_position;
		Rect m_rect;
		Event<MouseEventArgs> on_click;
		Event<PaintEventArgs> on_paint;
	public:

		Control() = default;
		~Control() = default;
	
		bool Enabled() const;
		void Enabled(bool value);
		
		String Name() const;
		void Name(String value);

		Point Position() const;
		void Position(Point value);

		void Select();
		void PerfomClick();

		void Show();
		void Hide();

		void Invalidate();
		void FindForm();
	};
}