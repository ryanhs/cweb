#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "../src/models/route.h"
#include "../src/helpers/handler.h"
#include "../src/helpers/wrapper.h"
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

void TEST_ROUTE(){
	//school_init();
	
	route_add("ping", method_ping);
	route_add("sum", method_sum);
	
	char request[][300] = {
		// 0
		"POST / HTTP/1.1\r\n"
		"Content-Type: application/json-rpc\r\n"
		"Content-Length: 69\r\n"
		"\r\n"
		"{\"jsonrpc\": \"2.0\", \"method\": \"sum\", \"params\": [1, 2, 3, 4], \"id\": 5}",
		
		// 1
		"GET / HTTP/1.1\r\n"
		"Content-Type: application/json-rpc\r\n"
		"Content-Length: 47\r\n"
		"\r\n"
		"{\"jsonrpc\": \"2.0\", \"method\": \"ping\", \"id\": 5}",
		
		// 2
		"POST / HTTP/1.1\r\n"
		"Content-Type: application/json-rpc\r\n"
		"Content-Length: 69\r\n"
		"\r\n"
		"{\"jsonrpc\": \"2.0\", \"method\": \"sum\", \"params\": [1, 2, 3, 4], \"id\": 5}",
		
		// 3
		"POST / HTTP/1.1\r\n"
		"Content-Type: application/json-rpc\r\n"
		"Content-Length: 51\r\n"
		"\r\n"
		"{\"jsonrpc\": \"2.0\", \"method\": \"not_found\", \"id\": 5}",
		
		// 4
		"POST / HTTP/1.1\r\n"
		"Content-Type: application/json-rpc\r\n"
		"Content-Length: 51\r\n"
		"\r\n"
		"{\"jsonrpc\": \"2.0\", \"method\": \"not_found\", \"id\": 5}",
		
		// 5
		"POST / HTTP/1.1\r\n"
		"Host: 127.0.0.1\r\n"
		"Content-Length: 55\r\n"
		"Connection: close\r\n"
		"\r\n"
		"{\"id\":1,\"method\":\"ping\",\"params\":[1,2],\"jsonrpc\":\"2.0\"}",
		
		// 6
		"POST / HTTP g\",\"params\":[1,2],\"jsonrpc\":\"2.0\"}",
		
		// 7
		"POST / HTTP/1.1\r\n"
		"Host: 127.0.0.1\r\n"
		"Content-Length: 55\r\n"
		"Connection: close\r\n"
		"\r\n"
		"",
		
		// 8
		"POST / HTTP/1.1\r\n"
		"Host: 127.0.0.1\r\n"
		"Content-Length: 55\r\n"
		"Connection: close\r\n"
		"\r\n"
		"{\"id\":1,\"method\":\"ping\",\"params\":[1,2],\"jsonrpc\":\"2.0\"}",
	};
	
	char *response;
	//printf("%s\n", handler_http(request[8]));
	int i, j, c = 0, arr_size = 9;
	for(i = 1; i <= 100; i++){
		for(j = 0; j < arr_size; j++){
			response = handler_http(request[j]);
			ASSERT(response != NULL);
			
			//printf("%s\n", response);
			free(response); // why free is doubled!
			
			c++;
			if(c % (20 * arr_size) == 0)
				printf("%d...\n", c);
		}
	}
}
