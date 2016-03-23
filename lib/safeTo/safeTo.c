#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "safeTo.h"

void rawUrlDecode(char *qs, char *rtn, int maxChar)
{
	int i = 0;
    for(; *qs != '\0'; qs++, rtn++) {
		if(i == maxChar){
			break;
		}

        if(*qs == '%') {
            *rtn = (char) ToHex(qs[1]) * 16 + ToHex(qs[2]);
            qs++;
            qs++;
        } else if(*qs == '+') {
            *rtn = ' ';
        } else
            *rtn = *qs;
    }

    rtn++;
    *rtn = '\0';
}

char* safeString(char *str)
{
	if(str == NULL)
		return "";
		
	if(*str == '\0')
		return "";
	
	return str;
}

int safeAtoi(char *str)
{
	if(str == NULL)
		return 0;

	if(strlen(str) == 0)
		return 0;

	return atoi(str);
}

float safeAtof(char *str)
{
	if(str == NULL)
		return 0;

	if(strlen(str) == 0)
		return 0;

	return atof(str);
}

void make2digit(char *rtn, int d)
{
	if(d < 10)
		sprintf(rtn, "0%d", d);
	else
		sprintf(rtn, "%d", d);
}

/*
**  TRIM.C - Remove leading, trailing, & excess embedded spaces
**
**  public domain by Bob Stout
*/
char *trim(char *str)
{
      char *ibuf = str, *obuf = str;
      int i = 0, cnt = 0;

      /*
      **  Trap NULL
      */
      if(str){
            /*
            **  Remove leading spaces (from RMLEAD.C)
            */
            for(ibuf = str; *ibuf && isspace(*ibuf); ++ibuf)
                  ;

            if(str != ibuf)
				memmove(str, ibuf, ibuf - str);

            /*
            **  Collapse embedded spaces (from LV1WS.C)
            */
            while (*ibuf){
                  if(isspace(*ibuf) && cnt)
                        ibuf++;
                  else{
                        if(!isspace(*ibuf))
                              cnt = 0;
                        else{
                              *ibuf = ' ';
                              cnt = 1;
                        }
                        obuf[i++] = *ibuf++;
                  }
            }

            obuf[i] = NUL;

            /*
            **  Remove trailing spaces (from RMTRAIL.C)
            */

            while (--i >= 0){
                  if (!isspace(obuf[i]))
                        break;
            }
            obuf[++i] = NUL;
      }
      return str;
}


void safeFree(void *ptr){
	if(ptr != NULL)
		free(ptr);
	ptr = NULL;
}

void *safeMalloc(size_t size){
	void *ptr = malloc(size);
	if(ptr == NULL)
		ptr = safeMalloc(size);
	
	return ptr;
}

void *safeCalloc(size_t num, size_t size){
	void *ptr = calloc(num, size);
	if(ptr == NULL)
		ptr = safeCalloc(num, size);
	
	return ptr;
}
