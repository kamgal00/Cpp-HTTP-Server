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
#include "server_socket.h"

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int ServerSocket::setup(const char *PORT, int BACKLOG)
{

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}

	if (_bind())
		return -1;

	freeaddrinfo(servinfo);

	if (p == NULL)
	{
		fprintf(stderr, "server: failed to bind\n");
		return -1;
	}

	if (listen(listen_sock, BACKLOG) == -1)
	{
		perror("listen");
		return -1;
	}

	return 0;
}

int ServerSocket::_bind()
{
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((listen_sock = socket(p->ai_family, p->ai_socktype,
					  p->ai_protocol)) == -1)
		{
			perror("server: socket");
			continue;
		}

		if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &yes,
			       sizeof(int)) == -1)
		{
			perror("setsockopt");
			return -1;
		}

		if (bind(listen_sock, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(listen_sock);
			perror("server: bind");
			continue;
		}

		break;
	}
	return 0;
}

int ServerSocket::get() const
{
	return listen_sock;
}

int ServerSocket::accept_connection()
{
	sin_size = sizeof their_addr;
	new_fd = accept(listen_sock, (struct sockaddr *)&their_addr, &sin_size);
	if (new_fd == -1)
	{
		perror("accept");
		return -1;
	}
	return new_fd;
}