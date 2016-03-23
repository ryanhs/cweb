#include <stdio.h>
#include <malloc.h>
#include <math.h>

#include "../lib/unitTesting/lib/unitTesting.h"
#include "../src/models/config.h"

void testDebug(){
	int debug = config_getBool("debug");
	ASSERT(debug == 1 || debug == 0);
	
	ASSERT(config_get("testing for not found") == NULL);
}

void testHttp(){
	ASSERT(config_get("http_host") != NULL);
	ASSERT(config_getInt("http_port") > 0);
	ASSERT(config_getInt("http_worker") >= 0);
}

void testMysql(){
	ASSERT(config_getBool("mysql_use_unix_socket") == 1 || config_getBool("mysql_use_unix_socket") == 0);
	ASSERT(config_get("mysql_host") != NULL);
	ASSERT(config_getInt("mysql_port") > 0);
	ASSERT(config_get("mysql_sock") != NULL);
	ASSERT(config_get("mysql_user") != NULL);
	ASSERT(config_get("mysql_pass") != NULL);
	ASSERT(config_get("mysql_name") != NULL);
}

void TEST_ROUTE(){
	config_init();
	
	TEST_CALL("debug", testDebug);
	TEST_CALL("http", testHttp);
	TEST_CALL("mysql", testMysql);
}
