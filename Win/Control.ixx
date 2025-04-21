#include "pch.h"

export module Control;

import win;


// alppppphhhhaaaaaa
export namespace win::ui
{
	struct MouseEventArgs
	{
		int x;
		int y;
	};
	struct KeyEventArgs
	{
		int key;
	};


	class Control : public Object<Control>
	{
	private:

		HWND m_hWnd;
		HWND m_parent;
		bool m_enabled;
		String m_name;
		Point m_position;
		Rect m_rect;

	private:

	

	public:

	
		bool Enabled() const;
		void Enabled(bool value);

		void Select();
		void PerfomClick();

		void Show();
		void Hide();

		void Invalidate();
		void FindForm();

		
	};
}