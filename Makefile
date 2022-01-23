all: server
server: client_handler.cpp epoll_set.cpp HTTPServer.cpp server_socket.cpp HTTP_parser.cpp cache.cpp
	g++ -g $^ -o server 
run: server
	./server
clean:
	rm server