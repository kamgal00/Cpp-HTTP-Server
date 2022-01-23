#include "client_handler.h"
#include <unistd.h>

int ClientHandler::enter_mainloop()
{
	get_dir();
	for (;;)
	{
		wait_for_messages();
		handle_messages();
	}
}

void ClientHandler::get_dir()
{
	char tmp[256];
	getcwd(tmp, 256);
	dir_path = tmp;
}

void ClientHandler::wait_for_messages()
{
	epoll_count = es.wait(events, MAX_EVENTS);

	if (epoll_count == -1)
	{
		perror("poll");
		exit(1);
	}
}

void ClientHandler::handle_messages()
{
	for (int i = 0; i < epoll_count; i++)
	{
		if (events[i].events & EPOLLIN)
		{
			if (events[i].data.fd == server_socket)
				handle_new_connection();
			else
			{
				sender_fd = events[i].data.fd;
				handle_request();
			}
		}
	}
}

void ClientHandler::handle_new_connection()
{
	int new_fd = ss.accept_connection();
	if (new_fd == -1)
		perror("accept");
	else
		es.add(new_fd, EPOLLIN);
}

void ClientHandler::handle_request()
{
	nbytes = recv(sender_fd, buff, sizeof buff, 0);

	if (nbytes <= 0 || nbytes == sizeof buff)
	{
		close_sender();
	}
	else
		respond_to_request();
}

void ClientHandler::close_sender()
{
	close(sender_fd);
	es.remove(sender_fd);
}

void ClientHandler::respond_to_request()
{
	buff[nbytes] = 0;
	if (pr.parse_request(buff))
	{
		close_sender();
		return;
	}
	if (pr.protocol != "HTTP/1.1")
	{
		close_sender();
		return;
	}

	if (pr.command == "GET")
	{
		handle_GET();
	}
	else if (pr.command == "HEAD")
	{
		handle_HEAD();
	}
	else
	{
		close_sender();
	}
}

void ClientHandler::handle_GET()
{
	if (create_header())
		return;

	send_message(&mes);
	send_message(file);
	optionally_close();
}

void ClientHandler::handle_HEAD()
{
	if (create_header())
		return;

	send_message(&mes);
	optionally_close();
}

int ClientHandler::create_header()
{
	if (!(file = c.load_file(dir_path + pr.path)))
	{
		handle_file_not_found();
		return -1;
	}
	mes = "";
	mes += "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(file->size()) + "\r\n";
	add_connection_header_field();
	mes += "\r\n";
	return 0;
}

void ClientHandler::handle_file_not_found()
{
	mes = "HTTP/1.1 404 Not Found\r\n";
	send_message(&mes);
	close_sender();
}

void ClientHandler::add_connection_header_field()
{
	if (pr.header_fields.find("Connection") != pr.header_fields.end() && pr.header_fields["Connection"] == "close")
	{
		mes += "Connection: close\r\n";
	}
	else
	{
		mes += "Connection: keep-alive\r\n";
	}
}

void ClientHandler::send_message(std::string *m)
{
	send(sender_fd, m->c_str(), m->size(), 0);
}

void ClientHandler::optionally_close()
{
	if (pr.header_fields.find("Connection") != pr.header_fields.end() && pr.header_fields["Connection"] == "close")
	{
		close_sender();
	}
}