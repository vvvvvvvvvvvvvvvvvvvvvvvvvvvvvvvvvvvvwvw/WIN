import win.threading;
import win.net;
import win.system;
import win.debug;
import win.io;
import win;
import TypeDef;
import Event;

int main()
{
	win::Buffer buffer("hello world");

	win::debug::Console::WriteLine(buffer.data<wchar_t>());

	return 0;
}