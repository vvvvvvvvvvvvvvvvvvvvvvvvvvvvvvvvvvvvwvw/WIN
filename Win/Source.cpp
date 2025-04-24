import win.threading;
import win.net;
import win.system;
import win.debug;
import win.io;
import win;
import TypeDef;
import Event;
import Control;

int main()
{
	win::Buffer buffer("hello world");

	win::debug::Console::WriteLine(buffer.data<wchar_t>());
	
	win::ui::Control control;
	control.Name("Test Control");
	control.Enabled(true);

	return 0;
}