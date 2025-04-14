#include <iostream>

import win.threading;
import win.net;
import win.system;

void client_handling(win::net::Socket client)
{
    // exo server
    while (true)
    {
        win::Buffer message = client.Receive();
        client.Send(message);
    }
}

int main()
{
   
    win::net::Socket server_socket(win::net::ipv4, win::net::tcp);
    server_socket.Bind("127.0.0.1", "8080");
    server_socket.Listen(10);

    win::threading::SystemThreadPool sys_pool;
    
    while (true)
    {
        auto client = server_socket.Accept();
        sys_pool.enqueue(client_handling, std::move(client));
    }
    return 0;
}
