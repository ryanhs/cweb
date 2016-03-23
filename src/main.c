#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <resolv.h>	
#include <signal.h>	

#include "models/config.h"
#include "models/route.h"

#include "helpers/signal.h"
#include "helpers/handler.h"
#include "helpers/wrapper.h"

#include "methods/util.h"

#include "childs/http.h"
#include "childs/dataRefresher.h"
#include "../lib/safeTo/safeTo.h"

int test();


int main(){
	handle_signal();
	
	config_init();
	
	/*
	// example
	category_init();
	post_init();
	*/
	
	// start refresher
	// refresher_init();
	// printf("refresher init...\n");
	
	// route method add
	route_add("ping", 					method_ping);
	
	printf("route added...\n");
	
	
	
	
	// http api server
	int tmp_param;
	printf("start service...\n");
	safeFree(http_multi_thread(&tmp_param));
	
	//raise(SIGINT);
	
	return 0;
}
