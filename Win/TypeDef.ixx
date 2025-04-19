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