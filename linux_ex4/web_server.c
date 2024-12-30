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

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CONNECTIONS 10

void *client_handler(void *arg);
void handle_request(int client_fd, char *request);
void send_response(int client_fd, const char *status, const char *body);
void send_file_response(int client_fd, const char *file_path);
int validate_request(const char *request, char *file_path);
void error_response(int client_fd, const char *status, const char *message);

int main(){
	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_len = sizeof(client_addr);

	// create socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd < 0){
		perror("Socket creation failed");
		exit(1);
	}

	// set server
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	//set port
	if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("Binding failed");
		close(server_fd);
		exit(1);
	}

	if(listen(server_fd, MAX_CONNECTIONS) < 0){
		perror("Listen failed");
		close(server_fd);
		exit(1);
	}

	printf("Server if running on port %d\n", PORT);
	
	while(1){
		// Accept client to connect
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
		if(client_fd < 0) {
			perror("Accept failed");
			continue;
		}
	
		// create thread to handle request
		pthread_t thread_id;
		int *client_socket = malloc(sizeof(int));
		*client_socket = client_fd;
		
		if(pthread_create(&thread_id, NULL, client_handler, client_socket) != 0) {
			perror("Thread creation failed");
			free(client_socket);
			close(client_fd);
		}

		pthread_detach(thread_id);
	}

	close(server_fd);
	return 0;
}

// client handle function
void *client_handler(void *arg) {
	int client_fd = *((int *)arg);
	free(arg);
		
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	
	int bytes_read = read(client_fd,  buffer, BUFFER_SIZE-1);
	if(bytes_read <= 0) {
		close(client_fd);
		return NULL;
	}

	printf("Received request: \n%s\n", buffer);
	handle_request(client_fd, buffer);

	close(client_fd);
	return NULL;
}


// handle client request
void handle_request(int client_fd, char *request) {
	char file_path[BUFFER_SIZE] = "./";
	
	if(!validate_request(request, file_path)) {
		error_response(client_fd, "400 Bad Request", "Invalid HTTP Request");
	} else {
		struct stat file_stat;
		if (stat(file_path, &file_stat) == -1 || S_ISDIR(file_stat.st_mode)) {
			error_response(client_fd, "404 Not Found", "File Not Found");
		} else {
			send_file_response(client_fd, file_path);
		}
	}
}


// validate HTTP request
int validate_request(const char *request, char *file_path) {
	char method[16], path[BUFFER_SIZE], protocol[16];
//	char resolved_path[PATH_MAX];
//	char root_dir[BUFFER_SIZE] = ".www";	

	if(sscanf(request, "%15s %1023s %15s", method, path, protocol) != 3) {
		return 0;	
	}

	if(strcmp(method, "GET") != 0 || strncmp(protocol, "HTTP/1.1", 8) != 0){
		return 0;
	}

//	snprintf(file_path, BUFFER_SIZE, "%s%s", root_dir, path);

//	if(realpath(file_path, resolved_path) == NULL){
//		return 0;
//	}

//	if(strncmp(resolved_path, root_dir, strlen(root_dir)) != 0){
//		return 0;
//	}

//	strncpy(file_path, resolved_path, BUFFER_SIZE);
//	return 1;


	// prevent path attach
	if(strstr(path, "..") != NULL){
		return 0;	
	}

	snprintf(file_path + 2, BUFFER_SIZE - 2, "%s", path + 1);
	return 1;
}


// send response
void send_response(int client_fd, const char *status, const char *body) {
	char header[BUFFER_SIZE];
	snprintf(header, BUFFER_SIZE,
		"HTTP/1.1 %s\r\n"
		"Content-Length: %lu\r\n"
		"Content-Type: text/html\r\n"
		"Connection: close\r\n\r\n",
		status, strlen(body));
	write(client_fd, header, strlen(header));
	write(client_fd, body, strlen(body));
}


// send file content
void send_file_response(int client_fd, const char *file_path) {
	int file_fd = open(file_path, O_RDONLY);
	if(file_fd < 0) {
		error_response(client_fd, "404 Not Found", "File Not Found");
	}

	char header[BUFFER_SIZE];
	snprintf(header, BUFFER_SIZE,
		"HTTP/1.1 200 OK %s\r\n"
		"Content-Type: text/html\r\n"
		"Connection: close\r\n\r\n");
	write(client_fd, header, strlen(header));

	char buffer[BUFFER_SIZE];
	ssize_t bytes_read;
	while((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
		write(client_fd, buffer, bytes_read);
	}

	close(file_fd);
}


void error_response(int client_fd, const char *status, const char *message) {
	char body[BUFFER_SIZE];
	snprintf(body, BUFFER_SIZE, "<h1>%s</h1><p>%s</p>", status, message);
	send_response(client_fd, status, body);
}
