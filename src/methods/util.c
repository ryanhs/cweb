#include <stdio.h>
#include <stdlib.h>
#include "../../lib/cJSON/cJSON.h"
#include "../helpers/handler.h"

#include "util.h"

int method_ping(cJSON *params, cJSON **result, char *error_msg){
	*result = cJSON_CreateString("ok");
	return 0;
}
