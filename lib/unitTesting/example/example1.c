#include "../lib/unitTesting.h"

void testHello1(){
	ASSERT_TRUE(1 == 1);
}

void testHello2(){
	ASSERT_LIKE("hello", "hello");
}

void TEST_ROUTE(){
	TEST_CALL("hello 1", testHello1);
	TEST_CALL("hello 2", testHello2);
}


