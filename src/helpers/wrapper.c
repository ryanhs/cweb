#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wrapper.h"

/*
// Example
cJSON *schoolJSON(school_item *school){
	if(school == NULL) return NULL;
	
	cJSON *root = cJSON_CreateObject();		
	cJSON_AddNumberToObject(root, "id", school->id);
	cJSON_AddNumberToObject(root, "code", school->code);
	cJSON_AddStringToObject(root, "level", school->level == SCHOOL_ELEMENTARY ? "elementary" : 
										   school->level == SCHOOL_JUNIOR ? "junior" : 
										   school->level == SCHOOL_SENIOR ? "senior" : "vocational");
	
	cJSON_AddStringToObject(root, "name", school->name);
	
	cJSON_AddStringToObject(root, "address", school->address);
	cJSON_AddStringToObject(root, "address_district", school->address_district);
	cJSON_AddStringToObject(root, "address_subdistrict", school->address_subdistrict);
	cJSON_AddNumberToObject(root, "address_rw", school->address_rw);
	cJSON_AddNumberToObject(root, "address_rt", school->address_rt);
	
	cJSON_AddNumberToObject(root, "is_border", school->is_border);
	cJSON_AddNumberToObject(root, "foreigner_percentage", school->foreigner_percentage);	// typo :-p
	return root;
}
*/

cJSON *pingJSON(){
	cJSON *root = cJSON_CreateObject();
	
	cJSON_AddNumberToObject(root, "ping", 1);	
	return root;
}

