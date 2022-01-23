#ifndef H_CLIENT_HANDLER
#define H_CLIENT_HANDLER

#include <sys/epoll.h>
#include "epoll_set.h"
#include "server_socket.h"
#include "HTTP_parser.h"
#include "cache.h"

#define MAX_EVENTS 10

class ClientHandler
{
	ServerSocket &ss;
	EpollSet &es;
	ParsedRequest pr;
	Cache c;

	std::string dir_path;

	const int server_socket;
	epoll_event events[MAX_EVENTS];
	char buff[65536];

	int epoll_count;
	int sender_fd;
	int nbytes;

	void get_dir();
	void wait_for_messages();
	void handle_messages();
	void handle_new_connection();
	void handle_request();
	void close_sender();
	void respond_to_request();

	std::string mes;
	std::string *file;
	void add_connection_header_field();
	void send_message(std::string *m);
	int create_header();

	void handle_GET();
	void handle_HEAD();
	void handle_file_not_found();
	void optionally_close();

public:
	ClientHandler(ServerSocket &ss, EpollSet &es) : ss(ss), es(es), server_socket(ss.get()) {}
	int enter_mainloop();
};

#endif