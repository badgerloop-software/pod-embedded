#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pthread.h>
#include <unistd.h>

#include "LVTCPSocket.h"


int main(int argc, char const *argv[]) 
{ 
	SetupLVTCPServer(9091);
	
	while(1){
		usleep(10000);
	}
} 

pthread_t TCPThread;

void SetupLVTCPServer(int port){
	
	if (pthread_create(&TCPThread, NULL, TCPLoop, NULL)){
		fprintf(stderr, "Error creating LV Telemetry thread\n");
	}
	 
}

void *TCPLoop(void *arg){
	
	(void) arg;

	int server_fd, new_socket, valread;
	int opt = 1;
		
	struct sockaddr_in address;
	int addrlen = sizeof(address);
		
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){ 
		fprintf(stderr, "Error creating socket FD\n");
		exit(EXIT_FAILURE); 
	} 
		
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												  &opt, sizeof(opt))){ 
		fprintf(stderr, "Error attaching socket\n");
		exit(EXIT_FAILURE); 
	}
		
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons(9091); 
		
	if (bind(server_fd, (struct sockaddr *)&address,  
									 sizeof(address))<0) { 
		fprintf(stderr, "Error binding port\n");
		exit(EXIT_FAILURE); 
	}
	
	if (listen(server_fd, 3) < 0){ 
		fprintf(stderr, "Error listening\n");
		exit(EXIT_FAILURE); 
	}
	
	printf("START\n");
	
	while(1){
		char buffer[1024] = {0}; 
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
								   (socklen_t*)&addrlen))<0){ 
			fprintf(stderr, "Error accepting a packet\n"); 
			exit(EXIT_FAILURE); 
		}
		
		valread = read( new_socket , buffer, 1024); 
		printf("RECEIVED: %s\n",buffer);  
		
		send(new_socket, (char*) "Hello there!" , strlen("Hello there!") , 0 ); 
	}
	
}