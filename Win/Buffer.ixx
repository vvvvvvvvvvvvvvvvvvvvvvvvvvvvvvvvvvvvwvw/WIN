#include "pch.h"

export module Buffer;

import Object;
import Error;
import String;

export namespace win
{
	struct Buffer : public Object<Buffer>
	{
	private:
		wchar_t* m_data{ nullptr };
		size_t m_len{ 0 };
		size_t m_capacity{ 0 };

	public:
		Buffer() = default;

		Buffer(const std::wstring& data);
		Buffer(const char* data);
		Buffer(size_t size);
		Buffer(const Buffer& other);
		Buffer& operator=(const Buffer& other);
		Buffer(Buffer&& other) noexcept;
		Buffer& operator=(Buffer&& other) noexcept;
		~Buffer();

		void resize(size_t size);
		void free();

		[[nodiscard]] size_t len() const;

		template<typename T = Char>
		[[nodiscard]] T* data();

		template<typename T = Char>
		[[nodiscard]] const T* data() const;
	};

	template<typename T>
	T* Buffer::data()
	{
		if (!m_data) throw debug::Error("Buffer is null");
		return reinterpret_cast<T*>(m_data);
	}
	template<typename T>
	const T* Buffer::data() const
	{
		if (!m_data) throw debug::Error("Buffer is null");
		return reinterpret_cast<T*>(m_data);
	}
}
