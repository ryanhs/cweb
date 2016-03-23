#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "../../lib/safeTo/safeTo.h"

int config_init_status = 0;
config_item config_table[CONFIG_ITEM_MAX];
int config_i = 0;

void config_init(){
	if(config_init_status == 1)
		return;
		
	config_init_status = 1;
	
    FILE *fp;
    long len;
    char *data;
	
	fp = fopen("config.json", "rb");
	if (fp == NULL)
		return;
		
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	data= (char*) safeMalloc(len + 1);
	fread(data, 1, len, fp);
	
	fclose(fp);
	
	cJSON *json = cJSON_Parse(data);
	cJSON *subitem = json->child;
	
	safeFree(data);
	while (subitem){
		if(subitem->type == cJSON_String)
			config_add(subitem->string, subitem->valuestring);
		
		if(subitem->type == cJSON_Number){
			sprintf(data, "%d", subitem->valueint);
			config_add(subitem->string, data);
		}
		
		if(subitem->type == cJSON_True)
			config_add(subitem->string, "1");
			
		if(subitem->type == cJSON_False)
			config_add(subitem->string, "0");
		
		subitem = subitem->next;
	}
	
	cJSON_Delete(json);
}

void config_add(char* key, char *value){
	memset(config_table[config_i].key, 0, 255);
	sprintf(config_table[config_i].key, "%s", safeString(key));
	
	memset(config_table[config_i].value, 0, 255);
	sprintf(config_table[config_i].value, "%s", safeString(value));
	
	config_i++;
}

char *config_get(char* key){
	int i;
	for(i = 0; i < config_i; i++){
		if(strcmp(config_table[i].key, key) == 0){
			return config_table[i].value;
			break;
		}
	}
	
	return NULL;
}

int config_getInt(char* name){
	char *value = config_get(name);
	return value == NULL ? 0 : safeAtoi(value);
}

double config_getDouble(char* name){
	char *value = config_get(name);
	return value == NULL ? 0.0 : safeAtof(value);
}

char *config_getString(char* name){
	return config_get(name);
}

short int config_getBool(char* name){
	return config_getInt(name) == 1;
}

short int config_getBoolean(char* name){
	return config_getBool(name);
}
