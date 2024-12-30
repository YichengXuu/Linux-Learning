#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CONNECTIONS 10
#define MAX_EVENTS 100

void set_nonblocking(int sock);
void handle_request(int client_fd);
void send_response(int client_fd, const char *status, const char *content_type,  const char *body);

int main(){
	int listen_fd, epoll_fd;
	struct sockaddr_in server_addr;
	int i;

	// create a socket
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(EXIT_FAILURE);
	}

	// Set socket options
	int opt = 1;
	if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0 ){
		perror("setsockopt error");
		close(listen_fd);
		exit(EXIT_FAILURE);
	}
	
	// Bind the socket
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("bind error");
		close(listen_fd);
		exit(EXIT_FAILURE);
	}

	//Listen for connection
	if(listen(listen_fd, 10) < 0) {
		perror("listen error");
		close(listen_fd);
		exit(EXIT_FAILURE);	
	}

	printf("Epoll Web Server is running on port %d\n", PORT);

	// Create epoll instance
	if((epoll_fd = epoll_create1(0)) < 0 ) {
		perror("epoll_create1 error");
		close(listen_fd);
		exit(EXIT_FAILURE);
	}

	// Add listen_fd tp epoll instance
	struct epoll_event ev, events[MAX_EVENTS];
	ev.events = EPOLLIN;
	ev.data.fd = listen_fd;
	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) < 0) {
		perror("epoll_ctl error");
		close(listen_fd);
		close(epoll_fd);
		exit(EXIT_FAILURE);
	}	

	while(1) {
		int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if(nfds<0) {
			perror("epoll_wait error");
			break;
		}

		for(i = 0; i < nfds; i++) {
			if(events[i].data.fd == listen_fd) {
				// Accept new connection
				int client_fd = accept(listen_fd, NULL, NULL);
				if(client_fd < 0){
					perror("accept error");
					continue;
				}
				set_nonblocking(client_fd);

				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = client_fd;
				if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
					perror("epoll_ctl error");
					close(client_fd);
				}
			} else {
				// Handle client request
				handle_request(events[i].data.fd);
			}
		}
	}


	close(listen_fd);
	close(epoll_fd);
	return 0;
}

void handle_request(int client_fd) {
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, sizeof(buffer));

	int bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
	if(bytes_read <= 0) {
		perror("recv error");
		close(client_fd);
		return;
	}

	// Parse HTTP GET request
	char request_path[BUFFER_SIZE];
	if(strncmp(buffer, "GET", 3) == 0) {
		sscanf(buffer, "GET %s HTTP/1.1", request_path);

		if(strstr(request_path, "..") != NULL) {
			// Prevent directory traversal attacks
			send_response(client_fd, "400 bad Request", "text/plain", "400 Bad Request");
		} else {
			char file_path[BUFFER_SIZE];
			snprintf(file_path, sizeof(file_path), ".%s", request_path);
			
			// special handle root path
			if(strcmp(request_path, "/") == 0) {
				snprintf(file_path, sizeof(file_path), "./index.html");
			}	

			// check file is existent
			if(access(file_path, F_OK) == 0 ){
				FILE *file = fopen(file_path, "r");
				if(file){
					char file_content[BUFFER_SIZE];
					size_t bytes_read = fread(file_content, 1, sizeof(file_content) - 1, file);
					file_content[bytes_read] = '\0';
					send_response(client_fd, "200 OK", "text/html", file_content);
					fclose(file);
				}else {
					send_response(client_fd, "404 Not Found", "text/html", "404 Not Found");
				}
			} else {
				send_response(client_fd, "404 Not Found", "text/html", "404 Not Found");
				}
		}
	} else {
		// Unsupported request method
		send_response(client_fd, "400 Bad Request", "text/plain", "400 Bad Request");
	}
	
	close(client_fd);	
}



// send response
void send_response(int client_fd, const char *status, const char *content_type, const char *body) {
	char response[BUFFER_SIZE];
	snprintf(response, sizeof(response),
		"HTTP/1.1 %s\r\n"
		"Content-Type: %s\r\n"
		"Content-Length: %lu\r\n"
		"\r\n"
		"%s",
		status, content_type, strlen(body), body);
	send(client_fd, response, strlen(response), 0);
}

void set_nonblocking(int sock) {
	int opts = fcntl(sock, F_GETFL);
	if(opts < 0){
		perror("fcntl(F_GETFL)");
		exit(EXIT_FAILURE);	
	}
	opts = (opts | O_NONBLOCK);
	if(fcntl(sock, F_SETFL, opts) < 0) {
		perror("fcntl(F_SETFL)");
		exit(EXIT_FAILURE);
	}
}	
