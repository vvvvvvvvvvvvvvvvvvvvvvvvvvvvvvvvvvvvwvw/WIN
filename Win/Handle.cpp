#include "pch.h"

module Handle;

namespace win
{
	Handle::Handle() : m_handle(INVALID_HANDLE_VALUE) {}

	Handle::Handle(HANDLE handle) : m_handle(handle) {}

	Handle::Handle(const Handle& handle)
		: m_handle(handle.Duplicate()) {
	}

	Handle::Handle(Handle&& handle) noexcept
		: m_handle(handle.Detach().Get()) {
	}

	Handle& Handle::operator=(const Handle& handle)
	{
		if (this != &handle)
		{
			Close();
			m_handle = handle.Duplicate();
		}
		return *this;
	}

	Handle& Handle::operator=(Handle&& handle) noexcept
	{
		if (this != &handle)
		{
			Close();
			m_handle = handle.m_handle;
			handle.m_handle = INVALID_HANDLE_VALUE;
		}
		return *this;
	}

	Handle::~Handle()
	{
		Close();
	}

	bool Handle::IsValid() const noexcept
	{
		return m_handle != INVALID_HANDLE_VALUE;
	}

	HANDLE Handle::Get() const noexcept
	{
		return m_handle;
	}

	void Handle::Close()
	{
		if (IsValid())
		{
			CloseHandle(m_handle);
			m_handle = INVALID_HANDLE_VALUE;
		}
	}

	Handle Handle::Detach()
	{
		HANDLE handle = m_handle;
		m_handle = INVALID_HANDLE_VALUE;
		return Handle(handle);
	}

	HANDLE Handle::Duplicate() const
	{
		HANDLE duplicateHandle = nullptr;
		if (IsValid() &&
			DuplicateHandle(GetCurrentProcess(), m_handle, GetCurrentProcess(), &duplicateHandle, 0, FALSE, DUPLICATE_SAME_ACCESS))
		{
			return duplicateHandle;
		}
		return INVALID_HANDLE_VALUE;
	}

	Handle::operator HANDLE() noexcept
	{
		return m_handle;
	}

	Handle::operator HANDLE() const noexcept
	{
		return m_handle;
	}

	Handle::operator bool() const noexcept
	{
		return IsValid();
	}
}
