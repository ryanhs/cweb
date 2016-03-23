#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "../lib/unitTesting/lib/unitTesting.h"
#include "../src/models/config.h"
#include "../src/helpers/wrapper.h"

void json_print_delete(cJSON *json){
	char *out;
	
	out = cJSON_Print(json);
	cJSON_Delete(json);
	
	printf("%s<br/>",out);
	free(out);
}

void TEST_ROUTE(){
	cJSON *test;
	
	int i;
	for(i = 1; i <= 500; i++){
		
		test = pingJSON();
		ASSERT(test != NULL);
		ASSERT(cJSON_GetObjectItem(test, "ping")->valueint == 1);
		cJSON_Delete(test);
		
		if(i % 100 == 0)
			printf("%d...\n", i);
	}
	return;	
}
