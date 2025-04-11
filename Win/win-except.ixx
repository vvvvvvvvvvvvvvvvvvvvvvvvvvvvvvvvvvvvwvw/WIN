#include <exception>

export module win_except;

export namespace win::exc
{
	class handle_exception : public std::exception {};
	class system_exception : public std::exception {};
	class socket_exception : public std::exception {};
	class file_exception : public std::exception {};
	class stream_exception : public std::exception {};
}