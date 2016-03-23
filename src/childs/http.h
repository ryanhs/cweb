#ifndef _CHILDS_HTTP_H
#define _CHILDS_HTTP_H

#include "../models/config.h"

#define HTTP_PORT	config_getInt("http_port")
#define HTTP_HOST	config_get("http_host")
#define MAX_BUF		1000
#define HTTP_WORKER	config_getInt("http_worker")

// ready for pthread child
void *http_single_thread(void *args);
void *http_multi_thread(void *args);


#endif
