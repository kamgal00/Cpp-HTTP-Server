#ifndef H_server_socket
#define H_server_socket

#include <iostream>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <utility>
#include <vector>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <utility>
#include <vector>
#include <map>
#include <sys/epoll.h>
#include <fstream>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>

class ServerSocket
{
	int listen_sock, new_fd;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	struct sigaction sa;
	int yes = 1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	int _bind();

public:
	int setup(const char *PORT, int BACKLOG);
	int get() const;
	int accept_connection();
};

#endif