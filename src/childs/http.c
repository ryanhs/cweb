#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include "../models/config.h"
#include "../helpers/handler.h"

#include "../../lib/http-parser/http_parser.h"
#include "../../lib/safeTo/safeTo.h"
#include "http.h"

int connection_count = 0;
int socket_desc;


void *http_single_thread(void *args){
	int *return_status = safeMalloc(sizeof(int));
	*return_status = 0;
	
	int client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[MAX_BUF];
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    //puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_port = htons( HTTP_PORT );
    inet_pton(AF_INET, HTTP_HOST, &server.sin_addr.s_addr);
    
    // Make it re use
    int yes = 1;
	if ( setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 )
	{
		perror("setsockopt");
        *return_status = 1;
        return return_status;
	}
	
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        //perror("bind failed. Error");
        *return_status = 1;
        return return_status;
    }
    //puts("bind done");
     
    if(DEBUG)
		printf("Bind %s:%d\n", HTTP_HOST, HTTP_PORT);     
		
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    if(DEBUG)
		puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    char data[MAX_BUF * 2];
    char *response = NULL;
    
    
    while(1){
		
		//accept connection from an incoming client
		client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
		if (client_sock < 0)
		{
			//perror("accept failed");
			continue;
		}
		//puts("Connection accepted");
		
		connection_count++;
		if(connection_count % 1000 == 0 && DEBUG){
			printf("%d connection.\n", connection_count);
		}
		
		memset(&data, 0, sizeof(data));
		read_size = recv(client_sock , client_message , MAX_BUF , 0);
		
		sprintf(data, "%s", safeString(client_message));
		
		//printf("%s\n", data);
		
		response = handler_http(data);
		//printf("%s\n", response);
		
		write(client_sock , response , strlen(response));
		safeFree(response);
		 
		if(read_size == 0)
		{
			//puts("Client disconnected");
			
		}
		else if(read_size == -1)
		{
			//perror("recv failed");
		}
		
		close(client_sock);
	}
	
	close(socket_desc);
    
	return return_status;
}

static void *http_worker_start(void *args){
	if(DEBUG)
		printf("\tthread runnning...\n");
	
	int *return_status = safeMalloc(sizeof(int));
	*return_status = 1;
	
	int client_sock, c, read_size;
    struct sockaddr_in client;
    char client_message[MAX_BUF];
    
    //socket_desc = *((int *) args);
    
    char data[MAX_BUF * 2];
    char *response;
    
    while(1){
		
		//accept connection from an incoming client
		client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
		if (client_sock < 0)
		{
			//perror("accept failed");
			continue;
		}
		//puts("Connection accepted");
		
		connection_count++;
		if(connection_count % 1000 == 0 && DEBUG){
			printf("%d connection.\n", connection_count);
		}
		
		memset(&data, 0, sizeof(data));
		read_size = recv(client_sock , client_message , MAX_BUF , 0);
		
		sprintf(data, "%s", safeString(client_message));
		
		//printf("%s\n", data);
		response = handler_http(data);
		//printf("%s\n", response);
		
		write(client_sock , response , strlen(response));
		safeFree(response);
		 
		if(read_size == 0)
		{
			//puts("Client disconnected");
			
		}
		else if(read_size == -1)
		{
			//perror("recv failed");
		}
		
		close(client_sock);
	}
	
	close(socket_desc);
	
	return return_status;
}

void *http_multi_thread(void *args){
	int *return_status = safeMalloc(sizeof(int));
	*return_status = 0;
	
    struct sockaddr_in server;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1 && DEBUG){
        printf("Could not create socket");
    }
    //puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_port = htons( HTTP_PORT );
    inet_pton(AF_INET, HTTP_HOST, &server.sin_addr.s_addr);
     
    // Make it re use
    int yes = 1;
	if ( setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 )
	{
		perror("setsockopt");
        *return_status = 1;
        return return_status;
	}

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        //perror("bind failed. Error");
        *return_status = 1;
        return return_status;
    }
    
    if(DEBUG)
		printf("Bind %s:%d\n", HTTP_HOST, HTTP_PORT);     
     
    //Listen
    listen(socket_desc , 3);
	
	
    pthread_t thread[HTTP_WORKER];
    int i;
    for(i = 0; i < HTTP_WORKER; i++){
		pthread_create(&thread[i], NULL, &http_worker_start, &socket_desc);
		printf("thread #%d created...\n", i);
	}
	
    for(i = 0; i < HTTP_WORKER; i++){
		pthread_join(thread[i], NULL);
	}
	
	close(socket_desc);
	
	return return_status;
}
