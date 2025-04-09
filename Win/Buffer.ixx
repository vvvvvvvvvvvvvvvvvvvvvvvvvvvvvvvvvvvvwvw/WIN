#include "pch.h"

export module Buffer;

import Object;
import String;

export namespace win
{
	struct Buffer : public Object<Buffer>
	{
	private:
		char* m_data{ nullptr };
		size_t m_len{ 0 };
	public:
		Buffer() = default;

		explicit Buffer(const String& data);

		explicit Buffer(char* data);

		Buffer(const Buffer& other);

		Buffer& operator=(const Buffer& other);

		Buffer(Buffer&& other) noexcept;

		Buffer& operator=(Buffer&& other) noexcept;

		[[nodiscard]] size_t len() const
		{
			return m_len;
		}

		
		template<typename T = Char>
		[[nodiscard]] T* data()
		{
			return reinterpret_cast<T*>(m_data);
		}

		template<typename T = Char>
		[[nodiscard]] const T* data() const
		{
			return reinterpret_cast<const T*>(m_data);
		}

		void free();

		~Buffer()
		{
			free();
		}
	};
}
