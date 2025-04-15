import win.threading;
import win.net;
import win.system;
import win.debug;
import win.io;
import win;

import TypeDef;

int main()
{
	win::TcpPacket packet;
	packet.code = 101;
	packet.m_version = 1;
	packet.required_version = 1;
	packet.m_data = win::Buffer("aboba").data<char>();


	win::Buffer buff(packet);

	win::TcpPacket* test = buff.data<win::TcpPacket>();

	test->code;

	return 0;
}