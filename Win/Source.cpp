import win;
import win.io;
import win.debug;
import win.system;
import win.threading;

using namespace win;

int main()
{
	io::File file("test.txt", true);

	io::StreamWriter sw(file.GetStream());
	sw.Write("Hello World!");

	
	io::MemoryMappedFile MMF(file);
	MMF.Write("AAAAAAAA");

	String result = MMF.ReadAll();
	dbg::Console::WriteLine(result);

	return 0;
}