#include "../lib/unitTesting.h"
#include "../inc/foobar.h"

void testBasic(){
	ASSERT(1 == 1);
	
	ASSERT(1.5 == 1.5);
	
	ASSERT(1.5 != 1);
	
	
	
	ASSERT_NOT(1 == 2);
}

void TEST_ROUTE(){
	TEST_CALL("basic", testBasic);
}


