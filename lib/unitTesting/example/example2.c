#include <stdio.h>
#include "../lib/unitTesting.h"

void testBool(){
	ASSERT_TRUE(1 == 1);
	ASSERT_FALSE(1 == 0);
}

void testString(){
	ASSERT_LIKE("hello", "hello");
	ASSERT_NOT_LIKE("hello", "hello world");
	
	ASSERT_LOWERCASE("hello");
	ASSERT_UPPERCASE("HELLO");
	
	ASSERT_PRINTABLE("just something to print");
}

void testNumber(){
	ASSERT_EQUALS(5, 5);
	ASSERT_NOT_EQUALS(5, 6);
	
	ASSERT_ZERO(0);
	ASSERT_ODD(5);
	ASSERT_EVEN(6);
	
	ASSERT_MORE_THAN(5, 3);
	ASSERT_MORE_THAN_EQUALS(3, 3);
	
	ASSERT_LEST_THAN(3, 5);
	ASSERT_LEST_THAN_EQUALS(5, 5);
}

void TEST_ROUTE(){
	TEST_CALL("bool", testBool);
	TEST_CALL("string", testString);
	TEST_CALL("number", testNumber);
}


