module;
#include "pch.h"

export module TypeDef;


export namespace win
{
	using l_int = LARGE_INTEGER;
	using word = WORD;
	using d_word = DWORD;
	using u_word = UWORD;
	using float128 = FLOAT128;
	using context = void*;
	template<typename RT, typename... ARGS>
	using Func = RT(__stdcall*)(ARGS...);

	

	struct Point
	{
		int x{ 0 };
		int y{ 0 };
		Point() = default;
		Point(int x, int y) : x(x), y(y) {}

		bool operator==(const Point& other) const
		{
			return (x == other.x && y == other.y);
		}
	};
	struct Rect
	{
		int x{ 0 };
		int y{ 0 };
		int width{ 0 };
		int height{ 0 };
		Rect() = default;
		Rect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
	};
	struct Size
	{
		int width{ 0 };
		int height{ 0 };
		Size() = default;
		Size(int width, int height) : width(width), height(height) {}
	};

	struct TcpStandart
	{
		int m_version{ 0 };
		int required_version{ 0 };
	};

	struct TcpPacket : public TcpStandart
	{
		char m_data[256]{ 0 };
		int code{ 0 };
	};

}