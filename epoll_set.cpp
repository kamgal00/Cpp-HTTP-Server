#include "epoll_set.h"

int EpollSet::setup()
{
	epoll_fd = epoll_create1(0);
	if (epoll_fd < 0)
		return -1;
	return 0;
}

int EpollSet::add(int fd, uint32_t events)
{
	struct epoll_event event;
	event.data.fd = fd;
	event.events = events;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event))
		return -1;
	return 0;
}

int EpollSet::remove(int fd)
{
	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr))
		return -1;
	return 0;
}

int EpollSet::wait(epoll_event events[], int max_events)
{
	return epoll_wait(epoll_fd, events, max_events, -1);
}

int EpollSet::mod(int fd, uint32_t events)
{
	struct epoll_event event;
	event.data.fd = fd;
	event.events = events;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event))
		return -1;
	return 0;
}