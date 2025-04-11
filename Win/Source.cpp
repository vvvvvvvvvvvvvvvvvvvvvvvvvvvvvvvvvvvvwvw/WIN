import win;
import win.io;
import win.debug;
import win.threading;


#include <mutex>
#include <string>

void Pool_Test()
{
	win::dbg::Console::Success(std::to_string(win::threading::Thread::Id()));
}

int main()
{
	win::threading::SystemThreadPool syth(10);
	win::threading::Signal signal(true, false);

	syth.enqueue(Pool_Test);
	syth.enqueue(Pool_Test);
	syth.enqueue(Pool_Test);
	syth.enqueue(Pool_Test);

	win::threading::Thread::Sleep(2000);

	syth.enqueue(Pool_Test);
	syth.enqueue(Pool_Test);
	syth.enqueue(Pool_Test);

	win::threading::Thread::Sleep(2000);

	syth.enqueue(Pool_Test);
	syth.enqueue(Pool_Test);

	win::threading::Thread::Sleep(2000);

	return 0;
}