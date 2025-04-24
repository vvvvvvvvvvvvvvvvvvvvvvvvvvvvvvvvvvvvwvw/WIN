#include "pch.h"

module Control;

namespace win::ui
{

    bool Control::Enabled() const { return m_enabled; }
    void Control::Enabled(bool value)
    {
		if (value != m_enabled)
		{
			m_enabled = value;
			EnableWindow(m_hWnd, m_enabled);
		}
    }
    String Control::Name() const { return m_name; }
    void Control::Name(String value)
    {
        if (value != m_name)
        {
			m_name = value;
			SetWindowText(m_hWnd, m_name.data());
        }
    }
    Point Control::Position() const { return m_position; }
    void Control::Position(Point value)
    {
        if (value != m_position)
        {
			m_position = value;
			MoveWindow(m_hWnd, m_position.x, m_position.y, m_rect.width, m_rect.height, TRUE);
        }
    }
    void Control::Select()
    {
        if (IsWindow(m_hWnd))
        {
            SetFocus(m_hWnd);
        }
    }
    void Control::PerfomClick()
    {
        if (IsWindow(m_hWnd))
        {
            SendMessage(m_hWnd, BM_CLICK, 0, 0);
        }
    }
    void Control::Show()
    {
        if (IsWindow(m_hWnd))
        {
            ShowWindow(m_hWnd, SW_SHOW);
        }
    }
    void Control::Hide()
    {
        if (IsWindow(m_hWnd))
        {
            ShowWindow(m_hWnd, SW_HIDE);
        }
    }
    void Control::Invalidate()
    {
        if (IsWindow(m_hWnd))
        {
            InvalidateRect(m_hWnd, nullptr, TRUE);
            UpdateWindow(m_hWnd);
        }
    }
    void Control::FindForm()
    {
        if (IsWindow(m_hWnd))
        {
            HWND hRoot = GetAncestor(m_hWnd, GA_ROOT);
            m_parent = hRoot;
        }
    }
}