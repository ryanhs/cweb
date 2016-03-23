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

#define MY_PORT		3000
#define MAXBUF		1000
#define THREAD_MAX	4

#include "../src/models/route.h"
#include "../src/helpers/handler.h"
#include "../lib/unitTesting/lib/unitTesting.h"
#include "../lib/http-parser/http_parser.h"

int method_ping(cJSON *params, cJSON **result, char *error_msg){
	*result = cJSON_CreateString("ok");
	return 0;
}

int method_sum(cJSON *params, cJSON **result, char *error_msg){
	if(!params) return RESPONSE_INVALID_PARAMS;
	
	cJSON *tmp;
	double sum = 0;
	if(params->type == cJSON_Array){
		tmp = params->child;
		while (tmp){
			if(tmp->type == cJSON_Number)
				sum += tmp->valueint;
			
			tmp = tmp->next;
		}
	}
	
	*result = cJSON_CreateNumber(sum);
	return 0;
}

int connection_count = 0;

void single_thread(){
	int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[MAXBUF];
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    //puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( MY_PORT );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        //perror("bind failed. Error");
        return;
    }
    //puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    char data[MAXBUF * 2];
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
		if(connection_count % 1000 == 0){
			printf("%d connection.\n", connection_count);
		}
		
		memset(&data, 0, sizeof(data));
		read_size = recv(client_sock , client_message , MAXBUF , 0);
		strcat(data, client_message);
		
		//printf("%s\n", data);
		response = handler_http(data);
		//printf("%s\n", response);
		
		write(client_sock , response , strlen(response));
		free(response);
		 
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
     
    return;
}


static void *thread_start(void *args){
	printf("\tthread runnning...\n");
	
	int socket_desc, client_sock, c, read_size;
    struct sockaddr_in client;
    char client_message[MAXBUF];
    
    socket_desc = *((int *) args);
    
    char data[MAXBUF * 2];
    char *response;
    
    int i = 0;
    while(1){
		i++;
		
		//accept connection from an incoming client
		client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
		if (client_sock < 0)
		{
			//perror("accept failed");
			continue;
		}
		//puts("Connection accepted");
		
		connection_count++;
		if(connection_count % 1000 == 0){
			printf("%d connection.\n", connection_count);
		}
		
		memset(&data, 0, sizeof(data));
		read_size = recv(client_sock , client_message , MAXBUF , 0);
		strcat(data, client_message);
		
		//printf("%s\n", data);
		response = handler_http(data);
		//printf("%s\n", response);
		
		write(client_sock , response , strlen(response));
		free(response);
		 
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
	
	return 0;
}

void multi_thread(){
	int socket_desc;
    struct sockaddr_in server;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    //puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( MY_PORT );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        //perror("bind failed. Error");
        return;
    }
    //puts("bind done");
     
     
    //Listen
    listen(socket_desc , 3);
	
	
    pthread_t thread[THREAD_MAX];
    int i;
    for(i = 0; i < THREAD_MAX; i++){
		pthread_create(&thread[i], NULL, &thread_start, &socket_desc);
	}
	
    for(i = 0; i < THREAD_MAX; i++){
		pthread_join(thread[i], NULL);
	}
	
	close(socket_desc);
}

void TEST_ROUTE(){
	route_add("ping", method_ping);
	route_add("sum", method_sum);
	
	//single_thread();
	//multi_thread();
}
