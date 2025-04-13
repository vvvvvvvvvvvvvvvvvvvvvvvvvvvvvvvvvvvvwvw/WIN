#include <iostream>
import win.threading;
import win.net;

void client_handling(win::net::Socket client)
{
    std::cout << "Client handled in system thread pool." << std::endl;
}

int main()
{
   
    win::net::Socket server_socket(win::net::ipv4, win::net::tcp);
    server_socket.Bind("127.0.0.1", "8080");
    server_socket.Listen(10);

 
    win::threading::StaticThreadPool sys_pool(1);

    
    while (true)
    {
        auto client = server_socket.Accept();
        sys_pool.enqueue(client_handling, std::move(client));
    }
    return 0;
}
