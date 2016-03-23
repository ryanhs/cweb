#ifndef SAFE_TO_H
#define SAFE_TO_H

char* safeString(char *str);
int safeAtoi(char *str);
float safeAtof(char *str);

void make2digit(char *rtn, int d);

char *trim(char *str);

void safeFree(void *ptr);
void *safeMalloc(size_t sz);
void *safeCalloc(size_t num, size_t size);


// Helper macro to convert two-character hex strings to character value
#define ToHex(Y) (Y >= '0' && Y <= '9' ? Y - '0' : Y - 'A' + 10)
#define NUL '\0'

#endif
