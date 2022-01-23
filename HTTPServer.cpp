#include "server_socket.h"
#include "epoll_set.h"
#include "client_handler.h"

#define PORT "8080"

#define BACKLOG 20

ServerSocket ss;
EpollSet es;
int server_socket;

void setup()
{
	if (ss.setup(PORT, BACKLOG))
		exit(1);

	server_socket = ss.get();

	if (es.setup())
		exit(1);

	if (es.add(server_socket, EPOLLIN))
		exit(1);
}

int main(void)
{
	setup();

	ClientHandler h(ss, es);

	h.enter_mainloop();

	return 0;
}