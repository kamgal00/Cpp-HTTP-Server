#ifndef H_EPOLL_SET
#define H_EPOLL_SET

#include <sys/epoll.h>

class EpollSet
{
	int epoll_fd;

public:
	int setup();
	int add(int fd, uint32_t events);
	int remove(int fd);
	int mod(int fd, uint32_t events);
	int wait(epoll_event events[], int max_events);
};

#endif