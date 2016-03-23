#include "../lib/unitTesting.h"
#include "../inc/foobar.h"

void testFoobar(){
	int a = 5;
	int b = 6;
	int c = foobarPlus(5, 6);
	
	ASSERT_EQUALS(c, 11);
}

void TEST_ROUTE(){
	TEST_CALL("foobar", testFoobar);
}


