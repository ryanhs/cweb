#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "../lib/unitTesting/lib/unitTesting.h"
#include "../src/models/config.h"
#include "../src/helpers/handler.h"
#include "../src/models/route.h"

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
				sum += tmp->valuedouble;
				
			tmp = tmp->next;
		}
	}
	
	*result = cJSON_CreateNumber(sum);
	return 0;
}

int method_error_msg(cJSON *param, cJSON **result, char *error_msg){
	strcpy(error_msg, "Error Message!");
	return 1;
}

int method_unknown_error(cJSON *params, cJSON **result, char *error_msg){
	return 99;
}

void json_print_delete(cJSON *json){
	char *out;
	
	out = cJSON_Print(json);
	cJSON_Delete(json);
	
	printf("%s<br/>",out);
	free(out);
}

void testInvalidJSON(){
	cJSON *res = handler_raw("{make it error on parsing}");
	cJSON *error = cJSON_GetObjectItem(res, "error");
	
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(cJSON_GetObjectItem(error, "code")->valueint == RESPONSE_PARSE_ERROR);
	ASSERT(strcmp(cJSON_GetObjectItem(error, "msg")->valuestring, RESPONSE_PARSE_ERROR_MSG) == 0);
	ASSERT(cJSON_GetObjectItem(res, "id")->type == cJSON_NULL);
	
	cJSON_Delete(res);
}

void testInvalidRequest(){
	cJSON *res = handler_raw("\"just some string\"");
	cJSON *error = cJSON_GetObjectItem(res, "error");
	
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(cJSON_GetObjectItem(error, "code")->valueint == RESPONSE_INVALID_REQUEST);
	ASSERT(strcmp(cJSON_GetObjectItem(error, "msg")->valuestring, RESPONSE_INVALID_REQUEST_MSG) == 0);
	ASSERT(cJSON_GetObjectItem(res, "id")->type == cJSON_NULL);
	
	cJSON_Delete(res);
}

void testInvalidRequestId(){
	cJSON *res, *error;
	
	// boolean
	res = handler_raw("{\"jsonrpc\": \"2.0\", \"method\": \"ping\", \"id\": true}");
	error = cJSON_GetObjectItem(res, "error");
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(cJSON_GetObjectItem(error, "code")->valueint == RESPONSE_INVALID_REQUEST);
	ASSERT(strcmp(cJSON_GetObjectItem(error, "msg")->valuestring, RESPONSE_INVALID_REQUEST_MSG) == 0);
	ASSERT(cJSON_GetObjectItem(res, "id")->type == cJSON_NULL);
	cJSON_Delete(res);
	
	// array
	res = handler_raw("{\"jsonrpc\": \"2.0\", \"method\": \"ping\", \"id\": []}");
	error = cJSON_GetObjectItem(res, "error");
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(cJSON_GetObjectItem(error, "code")->valueint == RESPONSE_INVALID_REQUEST);
	ASSERT(strcmp(cJSON_GetObjectItem(error, "msg")->valuestring, RESPONSE_INVALID_REQUEST_MSG) == 0);
	ASSERT(cJSON_GetObjectItem(res, "id")->type == cJSON_NULL);
	cJSON_Delete(res);
	
	// object
	res = handler_raw("{\"jsonrpc\": \"2.0\", \"method\": \"ping\", \"id\": {}}");
	error = cJSON_GetObjectItem(res, "error");
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(cJSON_GetObjectItem(error, "code")->valueint == RESPONSE_INVALID_REQUEST);
	ASSERT(strcmp(cJSON_GetObjectItem(error, "msg")->valuestring, RESPONSE_INVALID_REQUEST_MSG) == 0);
	ASSERT(cJSON_GetObjectItem(res, "id")->type == cJSON_NULL);
	cJSON_Delete(res);
	
	// number but fraction
	res = handler_raw("{\"jsonrpc\": \"2.0\", \"method\": \"ping\", \"id\": 8.8}");
	error = cJSON_GetObjectItem(res, "error");
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(cJSON_GetObjectItem(error, "code")->valueint == RESPONSE_INVALID_REQUEST);
	ASSERT(strcmp(cJSON_GetObjectItem(error, "msg")->valuestring, RESPONSE_INVALID_REQUEST_MSG) == 0);
	ASSERT(cJSON_GetObjectItem(res, "id")->type == cJSON_NULL);
	cJSON_Delete(res);
}

void testInvalidRequestElse(){
	cJSON *res, *error;
	
	// method not exists
	res = handler_raw("{\"jsonrpc\": \"2.0\", \"id\": 3}");
	error = cJSON_GetObjectItem(res, "error");
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(cJSON_GetObjectItem(error, "code")->valueint == RESPONSE_INVALID_REQUEST);
	ASSERT(strcmp(cJSON_GetObjectItem(error, "msg")->valuestring, RESPONSE_INVALID_REQUEST_MSG) == 0);
	ASSERT(cJSON_GetObjectItem(res, "id")->valueint == 3);
	cJSON_Delete(res);
	
	// method not string
	res = handler_raw("{\"jsonrpc\": \"2.0\", \"method\": 1, \"id\": 3}");
	error = cJSON_GetObjectItem(res, "error");
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(cJSON_GetObjectItem(error, "code")->valueint == RESPONSE_INVALID_REQUEST);
	ASSERT(strcmp(cJSON_GetObjectItem(error, "msg")->valuestring, RESPONSE_INVALID_REQUEST_MSG) == 0);
	ASSERT(cJSON_GetObjectItem(res, "id")->valueint == 3);
	cJSON_Delete(res);
	
	// version
	res = handler_raw("{\"jsonrpc\": \"2.1\", \"method\": \"ping\", \"id\": 3}");
	error = cJSON_GetObjectItem(res, "error");
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(cJSON_GetObjectItem(error, "code")->valueint == RESPONSE_INVALID_REQUEST);
	ASSERT(strcmp(cJSON_GetObjectItem(error, "msg")->valuestring, RESPONSE_INVALID_REQUEST_MSG) == 0);
	ASSERT(cJSON_GetObjectItem(res, "id")->valueint == 3);
	cJSON_Delete(res);
	
	// params not stucture
	res = handler_raw("{\"jsonrpc\": \"2.0\", \"method\": \"ping\", \"params\": 1, \"id\": 3}");
	error = cJSON_GetObjectItem(res, "error");
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(cJSON_GetObjectItem(error, "code")->valueint == RESPONSE_INVALID_REQUEST);
	ASSERT(strcmp(cJSON_GetObjectItem(error, "msg")->valuestring, RESPONSE_INVALID_REQUEST_MSG) == 0);
	ASSERT(cJSON_GetObjectItem(res, "id")->valueint == 3);
	cJSON_Delete(res);
	
	// empty array (batch)
	res = handler_raw("[]");
	error = cJSON_GetObjectItem(res, "error");
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(cJSON_GetObjectItem(error, "code")->valueint == RESPONSE_INVALID_REQUEST);
	ASSERT(strcmp(cJSON_GetObjectItem(error, "msg")->valuestring, RESPONSE_INVALID_REQUEST_MSG) == 0);
	ASSERT(cJSON_GetObjectItem(res, "id")->type == cJSON_NULL);
	cJSON_Delete(res);
}

void testInvalidParams(){
	cJSON *res, *error;
	
	res = handler_raw("{\"jsonrpc\": \"2.0\", \"method\": \"sum\", \"id\": 5}"); // params not inserted
	error = cJSON_GetObjectItem(res, "error");
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(cJSON_GetObjectItem(error, "code")->valueint == RESPONSE_INVALID_PARAMS);
	ASSERT(strcmp(cJSON_GetObjectItem(error, "msg")->valuestring, RESPONSE_INVALID_PARAMS_MSG) == 0);
	ASSERT(cJSON_GetObjectItem(res, "id")->valueint == 5);
	
	cJSON_Delete(res);
}

void testNotFound(){
	cJSON *res, *error;
	
	// method not exists
	res = handler_raw("{\"jsonrpc\": \"2.0\", \"method\": \"not existed method\", \"id\": 3}");
	error = cJSON_GetObjectItem(res, "error");
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(cJSON_GetObjectItem(error, "code")->valueint == RESPONSE_METHOD_NOT_FOUND);
	ASSERT(strcmp(cJSON_GetObjectItem(error, "msg")->valuestring, RESPONSE_METHOD_NOT_FOUND_MSG) == 0);
	ASSERT(cJSON_GetObjectItem(res, "id")->valueint == 3);
	cJSON_Delete(res);
}

void testUnknownError(){
	cJSON *res, *error;
	
	// method not exists
	res = handler_raw("{\"jsonrpc\": \"2.0\", \"method\": \"unkown_error\", \"id\": 5}");
	error = cJSON_GetObjectItem(res, "error");
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(cJSON_GetObjectItem(error, "code")->valueint == 99);
	ASSERT(strcmp(cJSON_GetObjectItem(error, "msg")->valuestring, RESPONSE_UNKNOWN_ERROR_MSG) == 0);
	ASSERT(cJSON_GetObjectItem(res, "id")->valueint == 5);
	cJSON_Delete(res);
}

void testErrorMessage(){
	cJSON *res, *error;
	
	// method not exists
	res = handler_raw("{\"jsonrpc\": \"2.0\", \"method\": \"error_msg\", \"id\": 5}");
	error = cJSON_GetObjectItem(res, "error");
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(cJSON_GetObjectItem(error, "code")->valueint == 1);
	ASSERT(strcmp(cJSON_GetObjectItem(error, "msg")->valuestring, "Error Message!") == 0);
	ASSERT(cJSON_GetObjectItem(res, "id")->valueint == 5);
	cJSON_Delete(res);
}

void testPing(){
	cJSON *res;
	
	res = handler_raw("{\"jsonrpc\": \"2.0\", \"method\": \"ping\", \"id\": \"100\"}");
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(strcmp(cJSON_GetObjectItem(res, "result")->valuestring, "ok") == 0);
	ASSERT(strcmp(cJSON_GetObjectItem(res, "id")->valuestring, "100") == 0);
	cJSON_Delete(res);
}

void testSum(){
	cJSON *res;
	
	//res = handler_raw("{\"jsonrpc\": \"2.0\", \"method\": \"sum\", \"params\": [1, 2, 3, 4, 0.1], \"id\": 5}");
	res = handler_raw("{\"jsonrpc\": \"2.0\", \"method\": \"sum\", \"params\": [1, 2, 3, 4, 0.1]}");
	ASSERT(strcmp(cJSON_GetObjectItem(res, "jsonrpc")->valuestring, "2.0") == 0);
	ASSERT(cJSON_GetObjectItem(res, "result")->valuedouble == 10.1);
	//ASSERT(cJSON_GetObjectItem(res, "id")->valueint == 5);
	ASSERT(cJSON_GetObjectItem(res, "id")->type == cJSON_NULL);	
	cJSON_Delete(res);
}

void testBatch(){
	cJSON *res, *child;
	
	res = handler_raw("["
					  "{\"jsonrpc\": \"2.0\", \"method\": \"sum\", \"params\": [2], \"id\": 1},"
					  "{\"jsonrpc\": \"2.0\", \"method\": \"sum\", \"params\": [4], \"id\": 2},"
					  "{\"jsonrpc\": \"2.0\", \"method\": \"sum\", \"params\": [6], \"id\": 3}"
					  "]");
					  
	ASSERT(res->type == cJSON_Array);
	ASSERT(cJSON_GetArraySize(res) == 3);
		
	int i = 1;
	child = res->child;
	while (child){
		
		ASSERT(strcmp(cJSON_GetObjectItem(child, "jsonrpc")->valuestring, "2.0") == 0);
		ASSERT(cJSON_GetObjectItem(child, "result")->valuedouble == (i * 2));
		ASSERT(cJSON_GetObjectItem(child, "id")->valueint == i);
		
		i++;
		child = child->next;
	}
	cJSON_Delete(res);
}

void testBatchInvalidOne(){
	cJSON *res, *child, *error;
	
	res = handler_raw("["
					  "{\"jsonrpc\": \"2.0\", \"method\": \"sum\", \"params\": [2], \"id\": 1},"
					  "{\"jsonrpc\": \"2.0\", \"method\": \"sum\", \"params\": [4], \"id\": 2},"
					  "{\"jsonrpc\": \"2.0\", \"id\": 3}" // invalid
					  "]");
	//return json_print_delete(res);
	ASSERT(res->type == cJSON_Array);
	ASSERT(cJSON_GetArraySize(res) == 3);
		
	int i = 1;
	child = res->child;
	while (child){
		if(i == 3){
			error = cJSON_GetObjectItem(child, "error");
			ASSERT(strcmp(cJSON_GetObjectItem(child, "jsonrpc")->valuestring, "2.0") == 0);
			ASSERT(cJSON_GetObjectItem(error, "code")->valueint == RESPONSE_INVALID_REQUEST);
			ASSERT(strcmp(cJSON_GetObjectItem(error, "msg")->valuestring, RESPONSE_INVALID_REQUEST_MSG) == 0);
			ASSERT(cJSON_GetObjectItem(child, "id")->valueint == 3);
		}else{
			ASSERT(strcmp(cJSON_GetObjectItem(child, "jsonrpc")->valuestring, "2.0") == 0);
			ASSERT(cJSON_GetObjectItem(child, "result")->valuedouble == (i * 2));
			ASSERT(cJSON_GetObjectItem(child, "id")->valueint == i);
		}
		i++;
		child = child->next;
	}
	cJSON_Delete(res);
}


void TEST_ROUTE(){
	route_add("ping", method_ping);
	route_add("sum", method_sum);
	route_add("unkown_error", method_unknown_error);
	route_add("error_msg", method_error_msg);
	
	// check all possible error
	TEST_CALL("invalid json", testInvalidJSON);
	TEST_CALL("invalid request not object/array", testInvalidRequest);
	TEST_CALL("invalid request (id)", testInvalidRequestId);
	TEST_CALL("invalid request (method, version)", testInvalidRequestElse);
	TEST_CALL("not insert params", testInvalidParams);
	TEST_CALL("not found", testNotFound);
	TEST_CALL("unknown error", testUnknownError);
	TEST_CALL("error message", testErrorMessage);
	
	// ping
	TEST_CALL("test ping", testPing);
	
	// sum
	TEST_CALL("test sum", testSum);
	
	// batch
	TEST_CALL("test batch request", testBatch);
	TEST_CALL("test batch invalid one", testBatchInvalidOne);
}
