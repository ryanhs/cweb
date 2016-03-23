#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>

#include "../lib/unitTesting/lib/unitTesting.h"
#include "../src/models/route.h"

int method_sum(cJSON *params, cJSON **result, char *error_msg){
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

int method_ehlo(cJSON *param, cJSON **result, char *error_msg){
	*result = cJSON_Duplicate(param, 1);
	return 0;
}

int method_error_msg(cJSON *param, cJSON **result, char *error_msg){
	strcpy(error_msg, "Error Message!");
	return 1;
}

void json_print_delete(cJSON *json){
	char *out;
	
	out = cJSON_Print(json);
	cJSON_Delete(json);
	
	printf("%s<br/>",out);
	free(out);
}

void testSum(){
	int data[2] = {5, 5};
	cJSON *param = cJSON_CreateIntArray(data, 2);
	cJSON *result;
	
	route_call("sum", param, &result, NULL);
	ASSERT(result->valuedouble == 10.0);
	
	cJSON_Delete(param);
	cJSON_Delete(result);
}

void testEhlo(){
	char name[] = "ryan";
	
	cJSON *param = cJSON_CreateObject();
	cJSON_AddStringToObject(param, "name", name);
	
	cJSON *result;
	
	route_call("ehlo", param, &result, NULL);
	ASSERT_LIKE(cJSON_GetObjectItem(result, "name")->valuestring, name);
	
	cJSON_Delete(param);
	cJSON_Delete(result);
}

void testNotFound(){
	cJSON *param = cJSON_CreateObject();
	cJSON *result;
	
	int status = route_call("test", param, &result, NULL);
	ASSERT(status == -1);
	
	cJSON_Delete(param);
}

void testErrorMsg(){
	cJSON *param = cJSON_CreateObject();
	cJSON *result;
	
	char error_msg[255];
	memset(error_msg, 0, sizeof(error_msg));
	int status = route_call("error_msg", param, &result, error_msg);
	
	ASSERT(status == 1);
	ASSERT_LIKE(error_msg, "Error Message!");
	
	cJSON_Delete(param);
}

void TEST_ROUTE(){
	route_add("sum", method_sum);
	route_add("ehlo", method_ehlo);
	route_add("error_msg", method_error_msg);
	
	TEST_CALL("sum", testSum);
	TEST_CALL("ehlo", testEhlo);
	TEST_CALL("not found", testNotFound);
	TEST_CALL("error message", testErrorMsg);
}
