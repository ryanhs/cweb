#ifndef _CONFIG_H
#define _CONFIG_H

#include "../../lib/cJSON/cJSON.h"

#define CONFIG_ITEM_MAX 20

typedef struct{
	char key[255];
	char value[255];
} config_item;

void config_init();

char *config_get(char* name);
int config_getInt(char* name);
double config_getDouble(char* name);
char *config_getString(char* name);
short int config_getBool(char* name);
short int config_getBoolean(char* name);

void config_add(char* key, char *value);


#define DEBUG config_getBool("debug")

#endif
