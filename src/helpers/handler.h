#ifndef _HANDLER_H
#define _HANDLER_H

#include "../../lib/cJSON/cJSON.h"

// struct id
typedef struct {
	short int type;
	char valuestring[10];
	int valueint;
} struct_id;

// id type
#define REQUEST_ID_NULL		1
#define REQUEST_ID_STRING	2
#define REQUEST_ID_NUMBER	3


// parsing request, batch or single
char *handler_http(char *request); // http request data -> return http response, don't forget to free() output
cJSON *handler_raw(char *request); // json string
cJSON *handler_cJSON(cJSON *request); // json parsed

// handler error
cJSON *handler_error(int code, char *msg, struct_id stid);



// pre defined error code (-32768 to -­32000)
#define RESPONSE_PARSE_ERROR			-32700
#define RESPONSE_PARSE_ERROR_MSG 		"Parse Error!"

#define RESPONSE_INVALID_REQUEST		-32600
#define RESPONSE_INVALID_REQUEST_MSG	"Invalid Request!"

#define RESPONSE_METHOD_NOT_FOUND		-32601
#define RESPONSE_METHOD_NOT_FOUND_MSG	"Method Not Found!"

#define RESPONSE_INVALID_PARAMS			-32602
#define RESPONSE_INVALID_PARAMS_MSG		"Invalid Params!"

#define RESPONSE_INTERNAL_ERROR			-32603
#define RESPONSE_INTERNAL_ERROR_MSG		"Internal Error!"

#define RESPONSE_SERVER_ERROR			-32000
#define RESPONSE_SERVER_ERROR_MSG		"Server Error!"

#define RESPONSE_UNKNOWN_ERROR_MSG		"Unknown Error!"

#endif
