#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "unitTesting.h"

// catcher
int test_count = 0;
int pass = 1;
char* errorTest;
char* currentTest;

void makeOutput(){
	if(pass == 1){
		printf("  <strong>Test passed: %d</strong><br/>", test_count);
	}else{
		printf("  <error>Error on: %s</error><br/>", errorTest);
	}
}

// main 
int main(){
	
	TEST_ROUTE();
	
	makeOutput();
	
	return pass == 1;
}

// test label
void TEST_CALL(char* label, void (*func)()){
	currentTest = label;
	func();
	
	if(pass == 1)
		printf("  ✓ %s<br/>", currentTest);
}


/*
 * 		ASSERT 
 */
void ASSERT(int assert){
	if(pass != 1) return;
	
	test_count++;
	
	if(assert != 1){
		errorTest = currentTest;
		pass = 0;
		makeOutput();
		abort();
	}
}

void ASSERT_NOT(int assert){
	if(pass != 1) return;
	
	test_count++;
	
	if(assert != 0){
		errorTest = currentTest;
		pass = 0;
		makeOutput();
		abort();
	}
}

/*
 * 		ASSERT BOOL
 */
void ASSERT_TRUE(int a){
	ASSERT(a);
}

void ASSERT_FALSE(int a){
	ASSERT(a == 0);
}

/*
 * 		ASSERT STRING
 */
void ASSERT_LIKE(char *a, char* b){
	ASSERT(strcmp(a, b) == 0);
}

void ASSERT_NOT_LIKE(char *a, char* b){
	ASSERT(strcmp(a, b) != 0);
}

void ASSERT_LOWERCASE(char *a){
	int tmp_pass = 1;
	
	int max = strlen(a);
	int i;
	for(i = 0; i < max; i++){
		if(tmp_pass == 1){
			tmp_pass = islower(a[i]) != 0;
		}
	}
	
	ASSERT(tmp_pass);
}

void ASSERT_UPPERCASE(char *a){
	int tmp_pass = 1;
	
	int max = strlen(a);
	int i;
	for(i = 0; i < max; i++){
		if(tmp_pass == 1){
			tmp_pass = isupper(a[i]) != 0;
		}
	}
	
	ASSERT(tmp_pass);
}

void ASSERT_PRINTABLE(char *a){
	int tmp_pass = 1;
	
	int max = strlen(a);
	int i;
	for(i = 0; i < max; i++){
		if(tmp_pass == 1){
			tmp_pass = isprint(a[i]) != 0;
		}
	}
	
	ASSERT(tmp_pass);
}

/*
 * 		ASSERT SIMPLE MATH
 */
void ASSERT_EQUALS(int a, int b){
	ASSERT(a == b);
}

void ASSERT_NOT_EQUALS(int a, int b){
	ASSERT(a != b);
}

void ASSERT_ZERO(int a){
	ASSERT(a == 0);
}

void ASSERT_ODD(int a){
	ASSERT((a % 2) == 1);
}

void ASSERT_EVEN(int a){
	ASSERT((a % 2) == 0);
}

void ASSERT_MORE_THAN(int a, int b){
	ASSERT(a > b);
}

void ASSERT_MORE_THAN_EQUALS(int a, int b){
	ASSERT(a >= b);
}

void ASSERT_LEST_THAN(int a, int b){
	ASSERT(a < b);
}

void ASSERT_LEST_THAN_EQUALS(int a, int b){
	ASSERT(a <= b);
}
