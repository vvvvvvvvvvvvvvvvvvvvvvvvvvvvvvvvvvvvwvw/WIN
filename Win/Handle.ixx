#include "pch.h"

export module Handle;

import Object;

export namespace win
{
	class Handle : public Object<Handle>
	{
	private:
		HANDLE m_handle = INVALID_HANDLE_VALUE;

	public:
		Handle();
		Handle(HANDLE handle);
		Handle(const Handle& other);
		Handle(Handle&& other) noexcept;

		Handle& operator=(const Handle& other);
		Handle& operator=(Handle&& other) noexcept;

		~Handle();

		bool IsValid() const noexcept;

		HANDLE Get() const noexcept;
		void Close();
		Handle Detach();
		HANDLE Duplicate() const;

		explicit operator bool() const noexcept;
		operator HANDLE() noexcept;
		operator HANDLE() const noexcept;
	};
}
