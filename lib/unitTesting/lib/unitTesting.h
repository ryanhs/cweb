#ifndef _UNIT_TESTING_H
#define _UNIT_TESTING_H

// test schema
void TEST_ROUTE();
void TEST_CALL(char* label, void (*func)(int, char **));

// assert basic
void ASSERT(int);
void ASSERT_NOT(int);

// assert boolean
void ASSERT_TRUE(int a);
void ASSERT_FALSE(int a);

// assert string equals
void ASSERT_LIKE(char *a, char* b); // like is for string, equals for number 
void ASSERT_NOT_LIKE(char *a, char* b);
void ASSERT_LOWERCASE(char *a);
void ASSERT_UPPERCASE(char *a);
void ASSERT_PRINTABLE(char *a);

// assert simple math
void ASSERT_EQUALS(int a, int b);
void ASSERT_NOT_EQUALS(int a, int b);
void ASSERT_ZERO(int a);
void ASSERT_ODD(int a);
void ASSERT_EVEN(int a);
void ASSERT_MORE_THAN(int a, int b);
void ASSERT_MORE_THAN_EQUALS(int a, int b);
void ASSERT_LEST_THAN(int a, int b);
void ASSERT_LEST_THAN_EQUALS(int a, int b);

#endif
