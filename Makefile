all: server
server.x: client_handler.cpp epoll_set.cpp HTTPServer.cpp server_socket.cpp HTTP_parser.cpp cache.cpp
	g++ -g $^ -o server.x 
run: server.x
	./server.x
clean:
	rm server.x