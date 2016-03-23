#ifndef _ROUTE_H
#define _ROUTE_H

#include "../../lib/cJSON/cJSON.h"

#define ROUTE_ITEM_MAX 50

typedef struct{
	char name[255];
	int (*func)(cJSON *, cJSON **, char *);
} route_item;

void route_add(char*, int (*func)(cJSON *, cJSON **, char *));

/*
 * if return -1 = not found
 * if return 0  = ok
 * else unknown error
 * 
 * param:
 *   - method name to call
 *   - params
 *   - result
 *   - error msg
 */
int route_call(char*, cJSON *, cJSON **, char *);

int route_count();

#endif
