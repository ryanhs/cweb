#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "../../lib/http-parser/http_parser.h"
#include "../../lib/safeTo/safeTo.h"
#include "../models/route.h"
#include "handler.h"

char *handler_http(char *request){
	//printf("data:<br/>%s<br/>===============<br/>", request);
	int all_set = 0;
	int data_max = 8000;
	char data[data_max];
	memset(data, 0, sizeof(char) * data_max);
	int data_i = 0;

	int on_headers_complete_cb (http_parser *p){
		return !(all_set = (p->method == 3));
	}

	int on_http_data_cb (http_parser *p, const char *buf, size_t len){
		if(data_i + len > data_max) return 1;
		
		data_i += len;
		strcat(data, buf);
		
		return 0;
	}
	
	http_parser_settings settings =
	{.on_message_begin = 0
	,.on_header_field = 0
	,.on_header_value = 0
	,.on_url = 0
	,.on_status = 0
	,.on_body = on_http_data_cb
	,.on_headers_complete = on_headers_complete_cb
	,.on_message_complete = 0
	};
	
	http_parser *parser = safeMalloc(sizeof(http_parser));
	http_parser_init(parser, HTTP_REQUEST);
	
	http_parser_execute(parser, &settings, request, strlen(request));
	http_parser_execute(parser, &settings, NULL, 0);
	
	//printf("data:<br/>%s<br/>===============<br/>", data);
	//printf("%d<br/>", data_i);
	safeFree(parser);
		
	struct_id stid;
	stid.type = REQUEST_ID_NULL;
	cJSON *responseJSON = (all_set == 0 || data_i == 0) ? handler_error(RESPONSE_INVALID_REQUEST, NULL, stid) : handler_raw(data);	
	
	memset(data, 0, sizeof(char) * data_max);
	
	char *responseData = cJSON_PrintUnformatted(responseJSON);
	cJSON_Delete(responseJSON);
	
	char *responseHeader = safeMalloc(sizeof(char) * 300);
	memset(responseHeader, 0, 300);
	sprintf(responseHeader, 
			"HTTP/1.1 200 OK\r\n"
			//"Content-Type: application/json-rpc\r\n"
			"Content-Type: application/json\r\n"
			"Content-Length: %u\r\n"
			"Access-Control-Allow-Origin: *\r\n\r\n", // make all origin acceptable
			(unsigned int) strlen(responseData)
			);
			
	int responseLength = strlen(responseHeader) + strlen(responseData);
	char *response = (char *) safeMalloc(responseLength + 10);
	memset(response, 0, responseLength);
	
	sprintf(response, "%s%s", responseHeader, responseData);
	safeFree(responseHeader);
	safeFree(responseData);
	
	return response;
}

cJSON *handler_error(int code, char *msg, struct_id stid){
	cJSON *root = cJSON_CreateObject();
	cJSON *error = cJSON_CreateObject();
	
	cJSON_AddNumberToObject(error, "code", code);
	switch(code){
		case RESPONSE_PARSE_ERROR:
			cJSON_AddStringToObject(error, "msg", RESPONSE_PARSE_ERROR_MSG);
			break;
		case RESPONSE_INVALID_REQUEST:
			cJSON_AddStringToObject(error, "msg", RESPONSE_INVALID_REQUEST_MSG);
			break;
		case RESPONSE_METHOD_NOT_FOUND:
			cJSON_AddStringToObject(error, "msg", RESPONSE_METHOD_NOT_FOUND_MSG);
			break;
		case RESPONSE_INVALID_PARAMS:
			cJSON_AddStringToObject(error, "msg", RESPONSE_INVALID_PARAMS_MSG);
			break;
		case RESPONSE_INTERNAL_ERROR:
			cJSON_AddStringToObject(error, "msg", RESPONSE_INTERNAL_ERROR_MSG);
			break;
		case RESPONSE_SERVER_ERROR:
			cJSON_AddStringToObject(error, "msg", RESPONSE_SERVER_ERROR_MSG);
			break;
		default:
			if(msg != NULL)
				cJSON_AddStringToObject(error, "msg", msg);
			else
				cJSON_AddStringToObject(error, "msg", "Unknown Error.");
			break;
	}
	
	cJSON_AddStringToObject(root, "jsonrpc", "2.0");
	cJSON_AddItemToObject(root, "error", error);
	switch(stid.type){
		case REQUEST_ID_NUMBER:
			cJSON_AddNumberToObject(root, "id", stid.valueint);
			break;
		case REQUEST_ID_STRING:
			cJSON_AddStringToObject(root, "id", stid.valuestring);
			break;
		default:
			cJSON_AddNullToObject(root, "id");
			break;
	}
	return root;
}

cJSON *handler_raw(char *request){
	struct_id stid;
	stid.type = REQUEST_ID_NULL;
	
	cJSON *requestParsed = cJSON_Parse(request);
	if(!requestParsed) return handler_error(RESPONSE_PARSE_ERROR, NULL, stid);
	
	cJSON *response = handler_cJSON(requestParsed);
	cJSON_Delete(requestParsed);
	return response;
}

cJSON *handler_cJSON(cJSON *request){
	cJSON *root, *tmp;
	
	struct_id stid;
	stid.type = REQUEST_ID_NULL;
	
	if(request->type == cJSON_Array){
		// empty array is invalid
		if(cJSON_GetArraySize(request) == 0) return handler_error(RESPONSE_INVALID_REQUEST, NULL, stid);
		
		root = cJSON_CreateArray();
		tmp = request->child;
		while (tmp){
			if(tmp->type == cJSON_Object)
				cJSON_AddItemToArray(root, handler_cJSON(tmp));
			else
				cJSON_AddItemToArray(root, handler_error(RESPONSE_INVALID_REQUEST, NULL, stid));
			tmp = tmp->next;
		}
		
		return root;
	}
	
	if(request->type != cJSON_Object) return handler_error(RESPONSE_INVALID_REQUEST, NULL, stid);
	
	cJSON *jsonrpc = cJSON_GetObjectItem(request, "jsonrpc");
	cJSON *method = cJSON_GetObjectItem(request, "method");
	cJSON *params = cJSON_GetObjectItem(request, "params");
	cJSON *tmp_id = cJSON_GetObjectItem(request, "id");
	
	if(tmp_id){
		if(tmp_id->type == cJSON_String){
			strcpy(stid.valuestring, tmp_id->valuestring);
			stid.type = REQUEST_ID_STRING;
		}else if(tmp_id->type == cJSON_NULL){
			stid.type = REQUEST_ID_NULL;
		}else if(tmp_id->type == cJSON_Number){
			if(tmp_id->valueint == tmp_id->valuedouble){ // not fraction :-){
				stid.valueint = tmp_id->valueint;
				stid.type = REQUEST_ID_NUMBER;
			}else{
				return handler_error(RESPONSE_INVALID_REQUEST, NULL, stid);
			}
		}else{
			return handler_error(RESPONSE_INVALID_REQUEST, NULL, stid);
		}
	}
	
	if(!jsonrpc || !method) return handler_error(RESPONSE_INVALID_REQUEST, NULL, stid);
	if(jsonrpc->type != cJSON_String) return handler_error(RESPONSE_INVALID_REQUEST, NULL, stid);
	if(strcmp(jsonrpc->valuestring, "2.0") != 0) return handler_error(RESPONSE_INVALID_REQUEST, NULL, stid);
	if(method->type != cJSON_String) return handler_error(RESPONSE_INVALID_REQUEST, NULL, stid);
	if(params){
		if(!(params->type == cJSON_Array || params->type == cJSON_Object)) return handler_error(RESPONSE_INVALID_REQUEST, NULL, stid);
	}
	
	// everything ok, lets check for routing
	cJSON *result;
	char error_msg[255]; // error_msg 255 is enough :-)
	memset(error_msg, 0, sizeof(error_msg));
	
	int return_status = route_call(method->valuestring, params, &result, error_msg);
	
	if(return_status == -1) return handler_error(RESPONSE_METHOD_NOT_FOUND, NULL, stid);
	if(return_status != 0){
		if(strlen(error_msg) == 0)
			return handler_error(return_status, RESPONSE_UNKNOWN_ERROR_MSG, stid);
		else
			return handler_error(return_status, error_msg, stid);
	}
	
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "jsonrpc", "2.0");
	
	if(result == NULL){
		cJSON_AddNullToObject(root, "result");
	}else{
		cJSON_AddItemToObject(root, "result", result);
	}
	
	if(tmp_id == NULL){
		cJSON_AddNullToObject(root, "id");
	}else{
		switch(stid.type){
			case REQUEST_ID_NUMBER:
				cJSON_AddNumberToObject(root, "id", stid.valueint);
				break;
			case REQUEST_ID_STRING:
				cJSON_AddStringToObject(root, "id", stid.valuestring);
				break;
			default:
				cJSON_AddNullToObject(root, "id");
				break;
		}
		
	}
	return root;
}
