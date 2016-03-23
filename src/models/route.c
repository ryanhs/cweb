#include <malloc.h>
#include <string.h>

#include "route.h"
#include "../../lib/safeTo/safeTo.h"

route_item route_table[ROUTE_ITEM_MAX];
int route_item_i = 0;

void route_add(char* name, int (*func)(cJSON *, cJSON **, char *)){
	memset(route_table[route_item_i].name, 0, 255);
	sprintf(route_table[route_item_i].name, "%s", safeString(name));
	
	route_table[route_item_i].func = func;
	route_item_i++;
}

int route_call(char* name, cJSON *params, cJSON **result, char *msg){
	int return_code = -1;
	
	int i = 0;
	for(i = 0; i < route_item_i; i++){
		if(strcmp(route_table[i].name, name) == 0){
			return_code = route_table[i].func(params, result, msg);
			break;
		}
	}
	
	return return_code;
}

int route_count(){
	return route_item_i;
}
